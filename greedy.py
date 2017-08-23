from cs50 import *

f=get_float("O hai! How much change is owed?\n")
while f<0:
    f=get_float("O hai! How much change is owed?\n")
count=0
i=int(f*100)
count=count+int(i/25)
i=i%25
count=count+int(i/10)
i=i%10
count=count+int(i/5)
i=i%5
count=count+int(i)
print(int(count))