from sympy import *
from sympy.matrices import *
from functools import reduce
import operator

for m in range(2, 15):
	A = Matrix(m - 1, m - 1, lambda i, j: (2 + i)/(1 + i) if i == j else 1)
	xs = A**-1*Matrix(m - 1, 1, [m]*(m - 1))
	x1 = m - sum(xs)
	p = x1*reduce(operator.mul, (xi**i for i, xi in enumerate(xs, 2)), 1)
	print("m:{} p:{}".format(m, floor(p)))

