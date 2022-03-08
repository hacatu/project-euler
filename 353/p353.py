import numpy as np
import itertools as itt

n = np.array([0, 0, 3])
a = np.array([2, 1, 2])
b = np.array([1, 2, 2])
c = np.array([2, 2, 1])
d = np.array([3, 0, 0])
e = np.array([0, 3, 0])
N = np.diag([1, 1, -1])

def sp_dist(r, a, b):
	return (np.arcsin(np.linalg.norm(np.cross(a, b))/(r*r))/np.pi)**2

def sp_path_len(r, pts):
	n = np.array([0, 0, r])
	l = 0
	for a, b in zip(itt.chain((n,), pts), itt.chain(pts, (N@n,))):
		l += sp_dist(r, a, b)
	return l

"""
print("The risk of ->d-> is", sp_path_len(3, (d,)))
print("The risk of ->a->-a-> is", sp_path_len(3, (a, N@a)))
print("The risk of ->c->-c-> is", sp_path_len(3, (c, N@c)))
print("The risk of ->a->d->-a-> is", sp_path_len(3, (a, d, N@a)))
print("The risk of ->c->d->-c-> is", sp_path_len(3, (c, d, N@c)))
print("The risk of ->a->-c-> is", sp_path_len(3, (a, N@c)))
print("The risk of ->a->-b-> is", sp_path_len(3, (a, N@b)))
print("The risk of ->a->e->-a-> is", sp_path_len(3, (a, e, N@a)))
print("The risk of ->a->c->-c->-a-> is", sp_path_len(3, (a, c, N@c, N@a)))
print("The risk of ->a->c->d->-c->-a-> is", sp_path_len(3, (a, c, d, N@c, N@a)))
"""

perfect_sqrts = {x*x: x for x in range(2**15)}

def pythag_triples(r):
	yield (r, 0, 0)
	sqr = r*r
	l = int(np.floor(np.sqrt(sqr/3)))
	for a in range(r - 1, l, -1):
		for b in range(min(a, int(np.floor(np.sqrt(sqr - a*a)))), int(np.ceil(np.sqrt((sqr - a*a)/2))) - 1, -1):
			c = perfect_sqrts.get(sqr - a*a - b*b)
			if c is not None:
				yield (a, b, c)

"""
for n in range(1, 16):
	pass

def make_crt_lut(a, b):
	return {(k%a, k%b): k for k in range(a*b)}

lift3x5 = make_crt_lut(3, 5)
for p3, p5 in itt.product(((0,0,0),(1,1,1),(1,1,2),(1,2,1),(2,1,1),(2,2,1),(2,1,2),(1,2,2),(2,2,2)), ((0,0,0),(2,1,0),(2,4,0),(3,1,0),(3,4,0))):
	print(tuple(lift3x5[x] for x in zip(p3, p5)))
"""
