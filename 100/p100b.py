#this would have been easier if you did b*(b-1)/n*(n-1)=1/2, then, x=2b-1, y=2n-1
from math import sqrt

def isSquare(n):
	r = int(sqrt(n))
	return r**2 == n

p = 21
n = 120

while True:
	d = 2*n**2 - 2*n +1
	while not isSquare(d):
		n += 1
		d = 2*n**2 - 2*n + 1
	if n > 1e12:
		break
	p, n = n, n**2//p

print((1 + sqrt(d))//2)

