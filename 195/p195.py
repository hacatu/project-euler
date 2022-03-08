# c**2 = a**2 + b**2 - ab
# c = k*(u**2 + 3v**2)
# a = k*(u**2 + 2uv - 3v**2)
# b = k*(-u**2 + 2uv + 3v**2)

# (u - 3v)(u + v) <= -1
# (x + 3y)(x - y) >= 1
# -(u - 3v)(u - v) <= sqrt(2/3)n

import matplotlib.pyplot as plt
import numpy as np
import operator
from math import sqrt

n = 5
fst = operator.itemgetter(0)
snd = operator.itemgetter(1)

def mediant(l, r):
	(a, b), (c, d) = l, r
	return a + c, b + d

def forall_stern_brocot(l, r, f):
	while True:
		m = mediant(l, r)
		if not f(m):
			break
		forall_stern_brocot(l, m, f)
		l = m

points = []

def print_admissible(q):
	u, v = q
	if (u - 3*v)*(u + v) > -1 or (u + 3*v)*(u - v) < 1 or 3*((u - 3*v)*(u - v))**2 > 2*n**2:
		return False
	points.append(q)
	return True

print_admissible((2, 1))
forall_stern_brocot((1, 1), (2, 1), print_admissible)
forall_stern_brocot((2, 1), (3, 1), print_admissible)

print(points)

def triangle_for_parameters(q, k):
	u, v = q
	c = k*(u**2 + 3*v**2)
	a = k*(u**2 + 2*u*v - 3*v**2)
	b = k*(-u**2 + 2*u*v + 3*v**2)
	r = k*(3*v - u)*(u - v)*sqrt(3/2)
	return a, b, c, r

for u, v in points:
	print("u={}, v={}".format(u, v))
	k = 1
	while True:
		a, b, c, r = triangle_for_parameters((u, v), k)
		if r > n:
			break
		if c**2 == a**2 + b**2 - a*b:
			print("   k={}:""({}, {}, {}, {})".format(k, a, b, c, r))
		else:
			print("   k={}:""({}, {}, {}, {}) is INVALID".format(k, a, b, c, r))
		k += 1

max_u = max(map(fst, points))
max_v = max(map(snd, points))
b_us = list(np.arange(0, max_u, 0.02))
b_vs = [(-u + sqrt(4*u**2 + 3))/3 for u in b_us]
a_us = list(np.arange(sqrt(3/4) + 0.001, min(max_u, max_v), 0.02))
a_vs = [(u + sqrt(4*u**2 - 3))/3 for u in a_us]
ct_us = list(np.arange(sqrt(sqrt(6)*n) + 0.001, min(max_u, max_v), 0.02))
ct_vs = [(2*u + sqrt(u**2 - sqrt(6)*n))/3 for u in ct_us]
cb_us = list(np.arange(sqrt(sqrt(6)*n) + 0.001, max_u, 0.02))
cb_vs = [(2*u - sqrt(u**2 - sqrt(6)*n))/3 for u in cb_us]

plt.scatter(*zip(*points))
plt.plot(b_us, b_vs, "g-")
plt.plot(a_us, a_vs, "r-")
plt.plot(ct_us, ct_vs, "b-")
plt.plot(cb_us, cb_vs, "b-")
plt.show()


