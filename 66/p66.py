from math import sqrt, floor

SCALE = 2

a0 = 0
a = []

def a_(i):
	if i == 0:
		return a0
	return a[(i - 1)%len(a)]

def isSquare(n):
	r = floor(sqrt(n))
	return n == r*r

def coef(r, n, d):
	return floor(n/(sqrt(r) - d))

def computePFrac(r):
	global a0, a
	n = r
	d = 0
	a0 = c = coef(r, n, d)
	a = []
	n0 = n = (r - d*d)/n
	d0 = d = c*n - d
	while True:
		c = coef(r, n, d)
		a.append(c)
		n = (r - d*d)//n
		d = c*n - d
		if n == n0 and d == d0:
			break

def isPellSolution(x, d, y):
	if (x*x - 1)%d:
		return 0
	if (x*x - 1)//d == y*y:
		return 1
	return 0

def findMinPellSolution(d):
	computePFrac(d)
	x1 = 0
	y1 = 1
	x = 1
	y = 0
	i = 0
	while True:
		x, x1 = a_(i)*x + x1, x
		y, y1 = a_(i)*y + y1, y
		if isPellSolution(x, d, y):
			return x
		i += 1

maxMinX = 0
maxMinD = 0
for d in range(2, 1001):
	if isSquare(d):
		continue
	minX = findMinPellSolution(d)
	if minX > maxMinX:
		maxMinX = minX
		maxMinD = d
print(maxMinD)

