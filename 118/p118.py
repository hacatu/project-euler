from math import log
from itertools import permutations, chain

dmrPrimes = [2, 13, 23, 1662803]

def __builtin_ctz(n):
	n &= -n
	return int(log(n, 2))

def isPrime(n):
	def witness(x, n, s):
		if x == 1 or x == n - 1:
			return 1
		for a in range(0, s - 1):
			x = x*x%n
			if x == 1:
				return 0
			if x == n - 1:
				return 1
		return 0
	if n%2 == 0:
		return n == 2
	if n == 1:
		return 0
	n -= 1
	s = __builtin_ctz(n)
	d = n>>s
	n += 1
	for a in dmrPrimes:
		if a >= n:
			break
		x = pow(a, d, n)
		if not witness(x, n, s):
			return 0
	return 1;

def ttoi(t):
	s = 0
	for d in t:
		s *= 10
		s += d
	return s

def pairwise(seq):
    items = iter(seq)
    last = next(items)
    for item in items:
        yield last, item
        last = item

def increasing(L):
    return all(x<y for x, y in pairwise(L))

def primeSets(permutation, partition):
	parts = ()
	for part in partition:
		parts += (ttoi(permutation[:part]),)
		permutation = permutation[part:]
	if not increasing(parts):
		return 0
	for part in parts:
		if not isPrime(part):
			return 0
	return 1

def partitions(n, m):
	for l in range(m, n//2 + 1):
		for partition in partitions(n - l, l):
			yield (l,) + partition
	yield (n,)

print(sum(primeSets(permutation, partition) for permutation in permutations(range(1, 10)) for partition in partitions(9, 1)))

