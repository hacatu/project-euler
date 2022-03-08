from random import randint
from math import factorial as fact

def amn_rand_single(a, m, n):
	x = [randint(1, a) for _ in range(n)]
	x.sort()
	a = 0
	i = 0
	while i < n:
		c = 1
		while i < n -1 and x[i] == x[i + 1]:
			i += 1
			c += 1
		i += 1
		if c > a:
			a = c
	return a >= m

def amn_rand(a, m, n, t):
	s = 0
	for _ in range(t):
		if amn_rand_single(a, m, n):
			s += 1
	return s/t

def partition(n, m):
	if m == 1:
		yield [(1, n)] if n > 0 else []
	else:
		for i in range(n//m + 1):
			for p in partition(n - m*i, m - 1):
				if i > 0:
					yield [(m, i)] + p
				else:
					yield p

def amn_comp_part(a, n, p):
	r = fact(n)
	for e, x in p:
		for _ in range(x):
			r *= a
			a -= 1
		r //= fact(e)**x*fact(x)
	return r

def amn_comp(a, m, n):
	return 1 - sum(amn_comp_part(a, n, p) for p in partition(n, m - 1))/a**n

"""
print("Randomized: {}".format(amn_rand(365, 3, 90, 1000)))
print("Calculated: {}".format(amn_comp(365, 3, 90)))
"""

def amkn_rand_single(a, m, k, n):
	x = [randint(1, a) for _ in range(n)]
	c = [0]*a
	x.sort()
	for i in x:
		for j in range(i - k, i + k + 1):
			c[j%a] += 1
	return max(c) >= m

def amkn_rand(a, m, k, n, t):
	s = 0
	for _ in range(t):
		if amkn_rand_single(a, m, k, n):
			s += 1
	return s/t

def amkn_extend_single(b, a, m, k):
	if len(b) == 0:
		return [[1]]
	t = [1]*(k + 1)
	l = 0
	for i in b[:-1:-1]:
		if i - b[-1] + 2*k < 0:
			break
		for j in range(0, min(i - b[-1] + 2*k, k) + 1, -1):
			if t[j] == m:
				l = j + 1
				break
			t[j] += 1
	return [b + [i] for i in range(b[-1] + l, b[-1] + k + 1)]

def amkn_extend(bs, a, m, k):
	return [c for b in bs for c in amkn_extend_single(b, a, m, k)]

print("Randomized: {}".format(amkn_rand(10, 3, 1, 7, 1000)))

