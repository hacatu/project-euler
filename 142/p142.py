"""
x + y + z
x > y > z
x + y, x - y, x + z, x - z, y + z, y - z are perfect squares.
a = x + y
b = x + z
c = y + z
x = (+ a + b - c)/2 = s - c
y = (+ a - b + c)/2 = s - b
z = (- a + b + c)/2 = s - a
s = x + y + z = (+ a + b + c)/2
c < b < a
note a, b, and c are square so actually loop over their square roots.
x - y = b - c
x - z = a - c
y - z = a - b
"""

from math import sqrt
from sys import exit

def isSquare(n):
	r = int(sqrt(n))
	return n == r*r

def firstC(a, b):
	c = int(sqrt(a*a - b*b))
	return c + 2 - (a + b + c)%2

a = 3
while True:
	for b in range(int(a/sqrt(2)), a):
		for c in range(firstC(a, b), b, 2):
			if isSquare(b*b - c*c) and isSquare(a*a - c*c) and isSquare(a*a- b*b):
				print((a*a + b*b + c*c)/2)
				exit(0)
	a += 1

