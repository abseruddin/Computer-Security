import binascii
import numpy as np
from BitVector import *
from time import time

mysbox=[]
myinversesbox=[]
def implement_sbox():
    a=np.zeros(256)
    AES_modulus=BitVector(bitstring='100011011')
    for i in range(256):
        if i==0:
            a[i]=99
        else:
            b=BitVector(intVal=i,size=8).gf_MI(AES_modulus,8)
            s1=b
            s2=b
            s3=b
            s4=b
            s=BitVector(intVal=0x63,size=8)^b^(b<<1)^(b<<1)^(b<<1)^(b<<1)
            a[i]=s.int_val()
    for i in range(256):
        s=BitVector(intVal=a[i],size=8)
        y=s.get_bitvector_in_hex()
        mysbox.append(y)
    print(mysbox)

def implement_inverse_sbox():
    a=np.zeros(256)
    AES_modulus=BitVector(bitstring='100011011')
    for i in range(256):
        if i<0:
            a[i]=82
        else:
            # b=BitVector(intVal=i,size=8).gf_MI(AES_modulus,8)
            b=BitVector(intVal=i,size=8)
            s1=b
            s2=b
            s3=b
            s4=b
            s=BitVector(intVal=0x05,size=8)^(b<<1)^(b<<2)^(b<<3)
            if s.int_val()==0:
                a[i]=0
            else:
                s=s.gf_MI(AES_modulus,8)
            # print(s)
                a[i]=s.int_val()
    for i in range(256):
        s=BitVector(intVal=a[i],size=8)
        y=s.get_bitvector_in_hex()
        myinversesbox.append(y)
    print(myinversesbox)
# implement_sbox()
#implement_inverse_sbox()
a=[1,2,3,4]
b=np.zeros(4,dtype=np.int)
print(a[2])
print(b[2])