from math import lcm

LCM = [1, 1]
for b in range(2, 33):
	LCM.append(lcm(LCM[-1], b))
	
print(sum((4**i-2)//LCM[i] - (4**i-2)//LCM[i+1] for i in range(1, 32)))

