from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(str(screen_name),50)
    if tweets==None:
        return redirect(url_for("index"))

    # TODO
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    score = analyzer.analyze(helpers.get_user_timeline(screen_name, count=50))
    pos=analyzer.pos
    neg=analyzer.neg
    neutral=analyzer.mid
    # generate chart
    chart = helpers.chart(pos, neg, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)




