from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    trans=db.execute("SELECT DISTINCT symbol,SUM(shares) as nos FROM transactions WHERE user_id = :uid and bors = :show group by symbol EXCEPT SELECT symbol,SUM(shares) as nos FROM transactions WHERE user_id = :uid AND bors = :hint group by symbol",uid=session["user_id"],show='BUY',hint='SELL')
    user=db.execute("SELECT * FROM users WHERE id = :uid",uid=session["user_id"])
    cur_price=[]
    for qwert in trans:
        cur_price.append(lookup(qwert["symbol"])["price"])
    
    return render_template("index.html",trans=trans,curp=cur_price,Id=user[0]["id"],username=user[0]["username"],value=user[0]["cash"])

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":
        # ensure symbol is correct
        if lookup(request.form.get("symbol")) == None:
            return apology("Enter a valid symbol")
        
        #read symbol ad get values into mydict
        sym=request.form.get("symbol")
        mydict=lookup(sym)
        
        #check if number they want to buy is valid
        if int(request.form.get("stock"))<=0:
            return apology("Enter a valid number")
            
        #store number
        num=request.form.get("stock")
        
        #check if they can afford
        tobuy=int(num)*int(mydict["price"])
        result=db.execute("SELECT cash FROM users WHERE id = :id",id=session["user_id"])
        mycash=int(result[0]["cash"])
        if mycash<tobuy:
            return apology("Sorry cannot afford.")
        else:
            #update transactions table
            db.execute("INSERT INTO transactions(user_id,symbol,unit_price,shares,total_amount,date,bors) VALUES(:userid,:sym,:up,:sha,:ta,datetime('now'),:brs)",userid=session["user_id"],sym=sym,up=mydict["price"],sha=num,ta=tobuy,brs='BUY')
            
            #update users table
            db.execute("UPDATE users SET cash = :cash WHERE id = :id",cash=mycash-tobuy,id=session["user_id"])
            return redirect(url_for("index"))
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    toshow=db.execute("SELECT * FROM transactions WHERE user_id=:id",id=session["user_id"])
    return render_template("history.html",lit=toshow)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        #get symbol
        sym=request.form.get("Symbol")
        
        #initializes a dictionary
        mydict=lookup(sym)
        
        #error msg
        if mydict == None:
            return apology("Symbol Doesnot exist")
        else:
            return render_template("quoted.html",name=mydict["name"],price=mydict["price"],symbol=mydict["symbol"])
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET","POST"])
def register():
    """Register user."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password1"):
            return apology("must provide password")
        
        # ensure confirm password was submitted
        elif not request.form.get("password2"):
            return apology("must provide password")
        
        # ensure password and confirm-password are same
        elif request.form.get("password1")!=request.form.get("password2"):
            return apology("passwords entered are not same")
        
        #encrypt the password
        hashed=pwd_context.hash(request.form.get("password1"))
        
        #insert values into db
        result=db.execute("INSERT INTO users(username,hash) VALUES(:username,:hash)",username=request.form.get("username"),hash=hashed)
        
        #if username already exists
        if not result:
            return apology("Username already exists")
        
        #after success
        return redirect(url_for("login"))
        
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "POST":
        sym=request.form.get("symbol")
        # ensure symbol is correct
        if lookup(sym) == None:
            return apology("Enter a valid symbol")
        
        #check if user buyed this stock
        chk=db.execute("SELECT DISTINCT symbol from transactions")
        ccd=0
        for i in chk:
            if sym==i["symbol"]:
                ccd=ccd+1
                break
        if ccd==0:
            return apology("You Don't have shares for this stock")
        
        #read symbol ad get values into mydict
        mydict=lookup(sym)
        
        #store number
        num=request.form.get("stock")
        
        #check if number they want to buy is valid
        if int(num)<=0:
            return apology("Enter a valid number")
            
        #sell all the shares
        alls=db.execute("SELECT SUM(shares) as nos,symbol from transactions group by symbol")
        for x in alls:
            if sym == x["symbol"]:
                if int(num)!= int(x["nos"]):
                    return apology("Should sell all shares")
                
    
        #sell the shares
        tosell=int(num)*int(mydict["price"])
        result=db.execute("SELECT cash FROM users WHERE id = :id",id=session["user_id"])
        mycash=int(result[0]["cash"])
        
        #increase my cash level
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",cash=mycash+tosell,id=session["user_id"])
        #update transactions table
        db.execute("INSERT INTO transactions(user_id,symbol,unit_price,shares,total_amount,date,bors) VALUES(:userid,:sym,:up,:sha,:ta,datetime('now'),:brs)",userid=session["user_id"],sym=sym,up=mydict["price"],sha=num,ta=tosell,brs='SELL')
        return redirect(url_for("index"))
    else:
        return render_template("sell.html")

@app.route("/repass", methods=["GET","POST"])
@login_required
def repass():
    """Show history of transactions."""
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("old"):
            return apology("must provide old password")

        # ensure password was submitted
        elif not request.form.get("new1"):
            return apology("must provide new password")
        
        # ensure confirm password was submitted
        elif not request.form.get("new2"):
            return apology("must confirm new password")
        
        # ensure password and confirm-password are same
        elif request.form.get("new1")!=request.form.get("new2"):
            return apology("new passwords entered are not same")
        
        #check old password is typed correct
        oldp=db.execute("SELECT * FROM users where id=:id",id=session["user_id"])
        if not pwd_context.verify(request.form.get("old"), oldp[0]["hash"]):
            return apology("Invalid Old Password")
        #encrypt the password
        hashed=pwd_context.hash(request.form.get("new1"))
        
        #insert values into db
        result=db.execute("UPDATE users SET hash=:hashed WHERE id=:id",id=session["user_id"],hashed=hashed)
        
        #if username already exists
        if not result:
            return apology("Could not change password")
        
        #after success
        return redirect(url_for("login"))
        
    else:
        return render_template("repass.html")