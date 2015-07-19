from fractions import Fraction
from linsolve import linsolve, pval

def optimum(p):
	eqns = [[Fraction(i**j) for j in range(p + 1)] + [ys[i - 1]] for i in range(1, p + 2)]
	return linsolve(eqns)

power = 10

#ns = [Fraction(n) for n in [0, 0, 0, 1]]

ns = [Fraction(-1 if i%2 else 1) for i in range(power + 1)]

ys = [pval(ns, x) for x in range(1, power + 1)]#starts at value for 1

s = Fraction(0)

for p in range(power):
	xs = optimum(p)
	m = p + 2
	#This could be needed sometimes, but it isn't needed here.
	#while pval(xs, m) == pval(ns, m):
	#	m += 1
	s += pval(xs, m)

print(s)

