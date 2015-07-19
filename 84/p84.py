from sympy import *
from fractions import Fraction
def d(n):
	if n == 2 or n == 8:#1+1; 4+4
		return Fraction(15, 256)
	elif n == 3 or n == 7:#1+2, 2+1; 3+4, 4+3
		return Fraction(1, 8)
	elif n == 4 or n == 6:#1+3, 2+2, 3+1; 2+4, 3+3, 4+2
		return Fraction(47, 256)
	elif n == 5:#1+4, 2+3, 3+2, 4+1
		return Fraction(1/4)
	return 0

#probability of reaching a from b,  ignoring all special conditions
def p(a, b):
	return d((a + 40 - b)%40)

#probability of reaching a from b, including all special conditions
def P(a, b):
	if a == 20:
		return 0
	pab = p(a, b)
	if a == 0:
		
	return pab

