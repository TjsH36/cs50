from cs50 import *

h=get_int("Height: ")
while h<=0 or h>23:
    if h==0:
        exit()
    else:
        h=get_int("Height: ")
for i in range(h):
    print(' '*(h-i-1)+'#'*(i+2))