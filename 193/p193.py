"""
squarefull numbers:
(2**2) + (3**2) + ... + (p**2)					#+singles
- ((2*3)**2) - ((2*5)**2) - ((2*p)**2)			#-doubles
+ ((2*3*5)**2) + ...							#+triples,
												#-quadruples,
												#+quintuples,
												#-sextuples,
(2*3*5*7*11*13*17)-(3*7*11*13*17*19*23)			#+septuples,
(2*3*5*7*11*13*17*19)-(2*3*5*11*13*17*19*23)	#-octuples
"""

import itertools as itt
import operator as op
from functools import reduce
from math import log, sqrt

def prod(it):
	return reduce(op.mul, it, 1)

def sqwheeld(*ps):
	return sum((-1)**n*sum(prod(l)**-2 for l in itt.combinations(ps, n)) for n in range(len(ps) + 1))

factorial = reduce(lambda a, n: a + [a[-1]*n], range(1, 41), [1])

M = 2**50
B = 2**22

bs = 2*log(log(M)/(2*log(B))) + 2/(sqrt(M)*log(M)) - 1/(B*log(B))
bs += 2*sum((-1)**n*((log(M)/2)**n - log(B)**n)/(n*factorial[n]) for n in range(1, 41))
bs *= B
print(bs)

