from math import floor, ceil
from fractions import Fraction as Q
from obelus import *

N = Var("N")
n = Var("n")

J = Expr.sum(n, floor(3*N/5)+1, N-1,
	floor((N-n)/2)*(floor((N-n)/2)+1) + (n+1-floor((N-n)/2))*(N-n)
)

#"""
print("Expression")
print(J)
J1 = Expr.normalize(J)
print("Simplified")
print(J1)
#"""

#print((N-1)._combine_add())

