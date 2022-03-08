from fractions import gcd
from math import sqrt

def lcm(a, b):
	return a*b//gcd(a, b)

p, q = 7, 11
n = p*q
cha = lcm(p - 1, q - 1)

ps = set()
is_composite = [False]*n

for x in range(2, n):
	if is_composite[x]:
		continue
	ps.add(x)
	for m in range(x*x, n, x):
		is_composite[m] = True

ps.remove(p)
ps.remove(q)

m = 0
am = 0

for e in range(3, cha, 2):
	if gcd(e, cha) != 1:
		continue
	"""
	count = 0
	for m in range(0, n):
		if pow(m, e, n) == m:
			count += 1
	print(count)
	"""
	#print("{}/{}".format(e, cha))
	image = {pow(p, e - 1, n) for p in ps}
	
	l = len(image)
	if l > m:
		m = l
		am = e
	elif l == m:
		am += e
	if len(image) == 15:
		print(e)
	
	
print(cha)
print(m)

