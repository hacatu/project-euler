"""
"""

import itertools as itt
from sympy import symbols, nsolve

K = 500
M = 10

def r_gen():
	r = 306
	while True:
		yield r
		r = r*r%10007

vs = symbols("v0:{}".format(K))
ps = [-M*v for v in vs]

for (i, j), r in zip(itt.product(range(K), range(M)), r_gen()):
	q = r%5
	if q == 0:
		ps[i] += 1
	elif q == 1:
		ps[i] += vs[i]**2
	elif q == 2:
		ps[i] += vs[2*i%K]
	elif q == 3:
		ps[i] += vs[(i*i+1)%K]**3
	elif q == 4:
		ps[i] += vs[i]*vs[(i+1)%K]

for p in ps:
	print(p)

slns = nsolve(ps, vs, (0,)*K)
#slns = [xs for xs in slns if 0 < xs[0] < 1]

print(slns[0])

