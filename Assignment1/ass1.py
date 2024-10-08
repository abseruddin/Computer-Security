import binascii
import numpy as np
from BitVector import *
from time import time

Sbox = (
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
)

InvSbox = (
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D,
)

Mixer = [
    [BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03")],
    [BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02")]
]

InvMixer = [
    [BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09")],
    [BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D")],
    [BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B")],
    [BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E")]
]

mixer_mult=np.zeros((256,4,4),dtype='U2')
invmixer_mult=np.zeros((256,4,4),dtype='U2')
def pre_calculation():
	AES_modulus = BitVector(bitstring='100011011')
	for i in range(256):
		for j in range(4):
			for k in range(4):
				s = Sbox[i]
				bv2 = BitVector(intVal=s, size=8)
				bv1= Mixer[j][k]
				bv3= bv1.gf_multiply_modular(bv2,AES_modulus,8)
				mixer_mult[i,j,k]=bv3.get_bitvector_in_hex()

				s1=InvSbox[i]
				bv5 = BitVector(intVal=s1, size=8)
				bv4= InvMixer[j][k]
				bv6= bv4.gf_multiply_modular(bv5,AES_modulus,8)
				invmixer_mult[i,j,k]=bv6.get_bitvector_in_hex()
				# if i==0 and j==0 and k==0:
				# 	print(mixer_mult[i,j,k]),
				# 	print(bv3.get_bitvector_in_hex())

	# print(mixer_mult)
"""Key scheduling"""
def byte_substitution(w):
	i=0
	p=""
	length=len(w)/2
	while i<length:
		b = BitVector(hexstring=w[i*2:(i+1)*2])
		int_val = b.intValue()
		s = Sbox[int_val]
		s = BitVector(intVal=s, size=8)
		s=s.get_bitvector_in_hex()
		p=p+s
		i=i+1
	return p

def add_round_constant(p,round_constant):
	a=BitVector(intVal=round_constant, size=8)
	a =a.get_bitvector_in_hex()
	a=str(a)
	while len(a)<8:
		a=a+"0"
	p=get_xor(p,a)	#adding round constant
	return p

def get_xor(w1,w2):
	i=0
	p=""
	length=len(w1)
	while i<length:
		b1 = BitVector(hexstring=w1[i])
		b2 = BitVector(hexstring=w2[i])
		#int_val1 = b1.intValue()
		#int_val2 = b2.intValue()
		#s=bool(int_val1)^bool(int_val2)
		s=b1^b2
		s =s.get_bitvector_in_hex()
		p=p+s
		i=i+1
	return p

s=[]
def key_schedule():
	#round_constant=320
	#if round_constant>320
	#print(a)
	key="BUET CSE16 Batch" #str(input("Enter the key: "))
	if len(key)>16:
		key=key[:16]
	elif len(key)<16:
		while len(key)<16:
			key=key+"0"
	# print(key)
	
	hex_code=binascii.hexlify(key.encode())
	hex_code=str(hex_code.decode())
	#print(hex_code)
	round_constant=1

	#s=[]
	s.append(hex_code)
	k=0
	while k<10:
		i=1
		w=[0,0,0,0,0]
		while i<5:
			j=(i-1)*8
			w[i]=hex_code[j:i*8]
			i=i+1
		i=2
		g=w[4][2:8]+w[4][0:2]	#circular shift
		g=byte_substitution(g)	#byte substitution 
		g=add_round_constant(g,round_constant)  #adding round constant
		w[1]=get_xor(w[1],g)	#
		#print(w[1])
		while i<5:
			w[i]=get_xor(w[i],w[i-1])
			i=i+1
		i=1
		p=""
		while i<5:
			p=p+str(w[i])
			i=i+1
		s.append(p)
		hex_code=p
		k=k+1
		round_constant=round_constant*2
		if round_constant>128:
			#round_constant=27
			round_constant=(round_constant)^(283)
			#print(round_constant)

def shift_rows(text):
	text1=text

	for i in range(1,4): #shift row
		for j in range(4):
			pos=i+j*4
			pos1=pos+i*4
			if pos1>16:
				pos1=pos1-16
			# print(pos)
			# print(pos1)
			pos=pos*2
			pos1=pos1*2
			text1=text1[:pos]+text[pos1:pos1+2]+text1[pos+2:]
	return text1


def mix_columns(text1):
	text=text1
	AES_modulus = BitVector(bitstring='100011011')
	for i in range(4):
		for j in range(4):
			result=""
			for k in range(4):
				# pos=(j*4+k)*2
				# bv1 = Mixer[i][k]
				# bv2 = BitVector(hexstring=text1[pos:pos+2])
				# bv3 = bv1.gf_multiply_modular(bv2, AES_modulus, 8)
				pos=(j*4+k)*2
				b = BitVector(hexstring=text1[pos:pos+2])
				int_val = b.intValue()
				s = InvSbox[int_val]
				s = BitVector(intVal=s, size=8)
				s = s.intValue()
				bv3 = BitVector(hexstring=mixer_mult[s,i,k])
				if k>0:
					result=get_xor(bv3.get_bitvector_in_hex(),result)
				else:
					result=bv3.get_bitvector_in_hex()
			pos=(i+j*4)*2
			text=text[:pos]+result+text[pos+2:]
	return text

def inverse_byte_substitution(w):
	i=0
	p=""
	length=len(w)/2
	while i<length:
		b = BitVector(hexstring=w[i*2:(i+1)*2])
		int_val = b.intValue()
		s = InvSbox[int_val]
		s = BitVector(intVal=s, size=8)
		s=s.get_bitvector_in_hex()
		p=p+s
		i=i+1
	return p

def inverse_mix_columns(text1):
	text=text1
	AES_modulus = BitVector(bitstring='100011011')
	for i in range(4):
		for j in range(4):
			result=""
			for k in range(4):
				# pos=(j*4+k)*2
				# bv1 = InvMixer[i][k]
				# bv2 = BitVector(hexstring=text1[pos:pos+2])
				# bv3 = bv1.gf_multiply_modular(bv2, AES_modulus, 8)
				pos=(j*4+k)*2
				b = BitVector(hexstring=text1[pos:pos+2])
				int_val = b.intValue()
				s = Sbox[int_val]
				s = BitVector(intVal=s, size=8)
				s = s.intValue()
				bv3 = BitVector(hexstring=invmixer_mult[s,i,k])
				if k>0:
					result=get_xor(bv3.get_bitvector_in_hex(),result)
				else:
					result=bv3.get_bitvector_in_hex()
			pos=(i+j*4)*2
			text=text[:pos]+result+text[pos+2:]
	return text

def inverse_shift_rows(text):
	text1=text

	for i in range(1,4): #shift row
		for j in range(4):
			pos=i+j*4
			pos1=pos+i*4
			if pos1>16:
				pos1=pos1-16
			# print(pos)
			# print(pos1)
			pos=pos*2
			pos1=pos1*2
			text1=text1[:pos1]+text[pos:pos+2]+text1[pos1+2:]
	return text1


def encryption_process(text,step):
	# print(text)
	text=get_xor(text,s[step]) #add round key
	text=byte_substitution(text) #substitution
	text1=shift_rows(text)
	text=mix_columns(text1)
	# text1=text

	# for i in range(1,4): #shift row
	# 	for j in range(4):
	# 		pos=i+j*4
	# 		pos1=pos+i*4
	# 		if pos1>16:
	# 			pos1=pos1-16
	# 		# print(pos)
	# 		# print(pos1)
	# 		pos=pos*2
	# 		pos1=pos1*2
	# 		text1=text1[:pos]+text[pos1:pos1+2]+text1[pos+2:]

	# text=text1
	# AES_modulus = BitVector(bitstring='100011011')
	# for i in range(4):
	# 	for j in range(4):
	# 		result=""
	# 		for k in range(4):
	# 			pos=(j*4+k)*2
	# 			# if j==0 and i==1:
	# 			# 	print(Mixer[i][k])
	# 			# 	print(text1[pos:pos+2])
	# 			bv1 = Mixer[i][k]
	# 			bv2 = BitVector(hexstring=text1[pos:pos+2])
	# 			bv3 = bv1.gf_multiply_modular(bv2, AES_modulus, 8)
	# 			if k>0:
	# 				result=get_xor(bv3.get_bitvector_in_hex(),result)
	# 			else:
	# 				result=bv3.get_bitvector_in_hex()
			# pos=(i+j*4)*2
			# text=text[:pos]+result+text[pos+2:]
		
	# print(text)
	# while i<11:
	# 	print(s[i])
	# 	i=i+1
	return text

def decryption_process(text,step):
	text=get_xor(text,s[step])
	text1=inverse_mix_columns(text)
	text=inverse_shift_rows(text1)
	text1=inverse_byte_substitution(text)
	return text1

def ROTL8(x,shift):
	p=x
	y=(x<<shift)&255
	z=(p>>(8-shift))&255
	w=y|z
	# return (((x) << (shift)) | ((x) >> (8 - (shift))))
	# print(w)
	return w
mysbox=[]
myinvsbox=[]
def implement_sbox():
	a=np.zeros(256)
	AES_modulus=BitVector(bitstring='100011011')
	# c=BitVector(bitstring='01100011')
	# for i in range(256):
	# 	if i==0:
	# 		q=BitVector(intVal=0)
	# 	else:
	# 		q=BitVector(intVal=i,size=8)
	# 	q=q.gf_MI(AES_modulus,8)
	# 	q1,q2,q3,q4=[q.deep_copy() for x in range(4)]
	# 	q=(q^ROTL8(q1,4)^ROTL8(q2,5)^ROTL8(q3,6)^ROTL8(q4,7)^c)&255
	# 	a[i]=q.intValue()
	p=1
	q=1
	num=0
	while p!=1 or num==0:
		num=num+1
		if p & 0x80:
			t=0x11B
		else:
			t=0
		# print(p)
		p = p^(p<<1)^t
		# print(p)
		# print(q)
		q ^= q << 1
		q=q&255
		q ^= q << 2
		q=q&255
		q ^= q << 4
		q=q&255
		if q & 0x80:
			r=0x09
		else:
			r=0
		q ^= r
		# print(q)
		xformed = (q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4))&255
		# print(a)
		# print(p)
		# print(xformed ^ 0x63)
		# print(xformed)
		a[p] = (xformed ^ 0x63)
		# print(p),
		# print(q),
		# print(a[p])

	a[0] = 0x63
	# print(a)
	for i in range(16):
		for j in range(16):
			s = BitVector(intVal=a[i*16+j], size=8)
			# a[i*16+j]=s.get_bitvector_in_hex()
			y=s.get_bitvector_in_hex()
			mysbox.append(y)
		# 	print(y),
		# print(".")
	# print(mysbox)


def inv_sbox():
	a=np.zeros(256)
	p=1
	q=1
	num=0
	while p!=1 or num==0:
		num=num+1
		
		# print(p)
		# print(q)
		q ^= q << 1
		q=q&255
		q ^= q << 2
		q=q&255
		q ^= q << 4
		q=q&255
		if q & 0x80:
			r=0x09
		else:
			r=0
		q ^= r
		# print(q)
		xformed = ( ROTL8(q, 1) ^ ROTL8(q, 3) ^ ROTL8(q, 6))&255
		
		# print(a)
		# print(p)
		# print(xformed ^ 0x63)
		# print(xformed)
		a[p] = (xformed ^ 0x05)
		if p & 0x80:
			t=0x11B
		else:
			t=0
		# print(p)
		p = p^(p<<1)^t
		# print(p),
		# print(q),
		# print(a[p])

	a[0] = 0x52
	# print(a)
	for i in range(16):
		for j in range(16):
			s = BitVector(intVal=a[i*16+j], size=8)
			# a[i*16+j]=s.get_bitvector_in_hex()
			y=s.get_bitvector_in_hex()
			myinvsbox.append(y)
		# 	print(y),
		# print(".")
	print(myinvsbox)


def encrypt_file():
	bv = BitVector( filename = 'a.pdf' )
	FILEOUT = open('output.bits', 'wb')
	while (bv.more_to_read):
		text = bv.read_bits_from_file( 128 )
		step=0
		# if len(plain_text)>=16:
		# 	text=plain_text[:16]
		# 	plain_text=plain_text[16:]
		if len(text)<128:
			text.pad_from_right(128-len(text))
		# 	plain_text=plain_text[len(plain_text):]

		# hex_code=binascii.hexlify(text.encode())
		# text=str(hex_code.decode())
		text=text.get_bitvector_in_hex()

		for i in range(9):
			text=encryption_process(text,step)
			step=step+1
		
		########final round###############
		text=get_xor(text,s[step])
		step=step+1
		text=byte_substitution(text)
		text=shift_rows(text)
		text=get_xor(text,s[step])
		bv1=BitVector(hexstring=text)
		bv1.write_to_file(FILEOUT)
		# print(text)
	FILEOUT.close()
	bv.close_file_object()


def decrypt_file():
	bv = BitVector( filename = 'output.bits' )
	# FILEOUT = open('output.txt', 'wb')
	file1 = open("b.pdf", "w") 
	decrypted_text=""
	while (bv.more_to_read):
		text = bv.read_bits_from_file( 128 )
		text=text.get_bitvector_in_hex()
		step=10
		text=get_xor(text,s[step])
		step=step-1
		text=inverse_shift_rows(text)
		text=inverse_byte_substitution(text)

		for i in range(9):
			# print(text)
			text=decryption_process(text,step)
			step=step-1
		
		########final round###############
		text=get_xor(text,s[step])
		step=step-1
		# text=byte_substitution(text)
		# text=shift_rows(text)
		# text=get_xor(text,s[step])
		# if (bv.more_to_read==False):
		# 	while text[len(text)-2:]=="00":
		# 		text=text[:len(text)-2]+"30"
		text=binascii.unhexlify(text.decode())
		file1.write(text)			
		decrypted_text=decrypted_text+text
	while decrypted_text[len(decrypted_text)-1]=="0":
		decrypted_text=decrypted_text[:len(decrypted_text)-2]

	# file1.write(decrypted_text)
	# print(decrypted_text)
	file1.close()
	bv.close_file_object()

implement_sbox()
inv_sbox()
pre_calculation()

start=time()
key_schedule()
end=time()
total_time=end-start
print(total_time)

encrypt_file()
print("done")
decrypt_file()
input("k")

plain_text="WillGraduateSoon" #str(input("Enter the text"))
cipher_text=""
step=0
start=time()
while len(plain_text)!=0:
	step=0
	if len(plain_text)>=16:
		text=plain_text[:16]
		plain_text=plain_text[16:]
	else:
		text=plain_text
		while len(text)<16:
			text=text+"0"
		plain_text=plain_text[len(plain_text):]

	hex_code=binascii.hexlify(text.encode())
	text=str(hex_code.decode())

	for i in range(9):
		text=encryption_process(text,step)
		step=step+1
	
	########final round###############
	text=get_xor(text,s[step])
	step=step+1
	text=byte_substitution(text)
	text=shift_rows(text)
	text=get_xor(text,s[step])
	cipher_text=cipher_text+text
	# print(text)
end=time()
total_time=end-start
print(total_time)
print(cipher_text)
step1=step
# cipher_text=binascii.unhexlify(cipher_text.decode())
# print(cipher_text)
############decryption##########
encrypted_text=cipher_text
decrypted_text=""
start=time()
while len(encrypted_text)!=0:
	step=step1
	if len(encrypted_text)>=32:
		text=encrypted_text[:32]
		encrypted_text=encrypted_text[32:]
	else:
		text=encrypted_text
		while len(text)<32:
			text=text+"0"
		encrypted_text=encrypted_text[len(encrypted_text):]

	# hex_code=binascii.hexlify(text.encode())
	# text=str(hex_code.decode())

	#######first round########
	text=get_xor(text,s[step])
	step=step-1
	text=inverse_shift_rows(text)
	text=inverse_byte_substitution(text)

	for i in range(9):
		# print(text)
		text=decryption_process(text,step)
		step=step-1
	
	########final round###############
	text=get_xor(text,s[step])
	step=step+1
	# text=byte_substitution(text)
	# text=shift_rows(text)
	# text=get_xor(text,s[step])
	# print(text)
	text=binascii.unhexlify(text.decode())
	decrypted_text=decrypted_text+text
while decrypted_text[len(decrypted_text)-1]=="0":
	decrypted_text=decrypted_text[:len(decrypted_text)-1]
end=time()
total_time=end-start
print(total_time)
print(decrypted_text)

###########task4#############
implement_sbox()
encrypt_file()
decrypt_file()