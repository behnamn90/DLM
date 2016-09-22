#!/usr/bin/env python

import os, sys
import numpy as np

myfile1 = sys.argv[1]

Temp = []
Prob = []
i=0
with open(myfile1,'r') as myfile:
	params = myfile.readline()
with open(myfile1,'r') as myfile:
	for line in myfile:
		if i!=0:
			Temp.append(int(line.split()[1]))
			Prob.append(float(line.split()[3])/166.)
		i+=1

T = []

if sys.argv[2] == "a":
	T_low = Temp[len(Temp)-1]
	T_high = Temp[0]
if sys.argv[2] == "m":
	T_low = Temp[0]
	T_high = Temp[len(Temp)-1]

for i in range(T_low,T_high+1):
	T.append(i)

P = []
i = 0
val = 0

for temp in T:
	Pi = []
	for k in range(0,len(Temp)):
		if Temp[k] == temp:
			Pi.append(Prob[k])
	P.append(Pi)


T_final = []
P_final = []
P_var = []
for i in range(0,len(T)):
	T_final.append(T[i])
	P_final.append(np.mean(P[i]))
	P_var.append(np.var(P[i]))

for i in range(0,len(T)):
	print(str(T_final[i]) +"\t"+ str(P_final[i]) +"\t"+ str(P_var[i]) )

T80=0
T50=0
T20=0
for i in range(0,len(T)):
	if P_final[i] < 0.2:
		T20 = T_final[i]
		break
for i in range(0,len(T)):
	if P_final[i] < 0.5:
		T50 = T_final[i]
		break
for i in range(0,len(T)):
	if P_final[i] < 0.8:
		T80 = T_final[i]
		break
	
print("T_"+sys.argv[2]+" = "+str(T50) + "\t" + "dT = "+str(abs(T80-T20))+ "\n" + params)
