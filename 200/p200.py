from heapq import heappush, heapreplace
from itertools import islice, takewhile

dmrPrimes = [2, 13, 23, 1662803]
N = 10

def __builtin_ctz(n):
	n &= -n
	return n.bit_length() - 1

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


def primes():
	g = [(4, 2)]
	n = 2
	while True:
		if n < g[0][0]:
			heappush(g, (n*n, n))
			yield n
			n += 1
		elif n > g[0][0]:
			heapreplace(g, (g[0][0] + g[0][1], g[0][1]))
		else:
			n += 1
			heapreplace(g, (g[0][0] + g[0][1], g[0][1]))

class Indexable(object):#from @unutbu on stackoverflow
	
	def __init__(self, it):
		self.it = iter(it)
		self.already_computed = []
	
	def __iter__(self):
		for elt in self.it:
			self.already_computed.append(elt)
			yield elt
	
	def __getitem__(self, index):
		try:
			max_idx = index.stop
		except AttributeError:
			max_idx = index
			n = max_idx - len(self.already_computed) + 1
			if n > 0:
				self.already_computed.extend(islice(self.it, n))
			return self.already_computed[index]

ppows = Indexable((p**2, p**3) for p in primes())

def squbes():
	g = [(ppows[1][0]*ppows[0][1], 1, 0), (ppows[0][0]*ppows[1][1], 0, 1)]#[(i, j, p**2*q**3)]
	while True:
		yield g[0][0]
		i = g[0][1]
		j = g[0][2]
		if g[0][1] + 2 == g[0][2] or g[0][2] + 2 == g[0][1]:
			heapreplace(g, (ppows[i + 1][0]*ppows[j][1], i + 1, j))
			heappush(g, (ppows[i][0]*ppows[j + 1][1], i, j + 1))
		elif g[0][1] > g[0][2]:
			heapreplace(g, (ppows[i + 1][0]*ppows[j][1], i + 1, j))
		else:
			heapreplace(g, (ppows[i][0]*ppows[j + 1][1], i, j + 1))

def has_200(n):
	while n >= 200:
		if n%1000 == 200:
			return True
		n //= 10
	return False

def is_prime_proof(n):
	t = 1
	tn = 10
	while t < n:
		d = n%tn//t
		for m in range(10):
			if m == d:
				continue
			if isPrime(n + (m - d)*t):
				return False
		t = tn
		tn *= 10
	return True

print(next(islice(filter(lambda s: has_200(s) and is_prime_proof(s), squbes()), 199, 200)))

