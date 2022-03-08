"""
c(n) = n^2 - sum(n=1,floor(sqrt(n)), c(m)(floor(n/m) - floor(n/m)) - sum(k=2,floor(n/k)>sqrt(n), c(floor(n/k)))
c(n) is the number of pairs 1 <= x, y <= n with gcd(x, y) = 1, but it counts (1,1) and it counts x > y, so
we need to adjust it slightly to (c(n)-1)/2.

Now the number of 2-friendly pairs <= n is the number of coprime pairs <= n/4 plus the number of coprime pairs
<= n/16 and so on, sum(t=1,..., (c(n/4^t)-1)/2
"""

from functools import lru_cache
from math import sqrt, floor

class CoprimeCounter:
	def __init__(self, n):
		self.dense_limit = floor(sqrt(n))
		self.dense_res = [None]*(self.dense_limit+1)
		self.sparse_res = {}

@lru_cache(maxsize=None)
def c(n):
	res = n*n
	res -= sum(c(m)*(n//m - n//(m+1)) for m in range(1, floor(sqrt(n)) + 1))
	res -= sum(c(n//k) for k in range(2, 1 + n//(floor(sqrt(n)) + 1)))
	return res

print(c(10**7))

