from cs50 import *
from sys import *

if len(argv)!=2:
    print("No comand line input")
    exit(1)
k=int(argv[1])
k=k%26
otp=[]
inp=get_string("plaintext: ")
for i in range(len(inp)):
    ind=ord(inp[i])
    if ind>=97 and ind<=122:
        if ind+k>122:
            nid=(ind+k)%123+97
        else:
            nid=(ind+k)
        otp.append(chr(nid))
    elif ind>=65 and ind <=90:
        if ind+k>90:
            nid=(ind+k)%91+65
        else:
            nid=ind+k
        otp.append(chr(nid))
    else:
        otp.append(inp[i])
print("ciphertext: "+''.join(otp))
