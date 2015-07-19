def memoize(f):
	class cache:
		def __missing__(self, x):
			ret = self[x] = f(x)
			return ret
	return cache().__get__

sigma = [0]*2 + [1]*999999

for d in range(2, 1000001):
	sigma[d**2] += d
	for m in range((d)*(d + 1), 1000001, d):
		sigma[m] += d + m//d

@memoize
def cyclen(n):
	s = sigma[n]
	if s > 1000001:
		return 0
	if cyclen(s) == 0:
		return 0
	

