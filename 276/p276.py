from functools import lru_cache
from math import gcd

def triag(n):
	n += 1
	if n&1:
		return (n*(n*(n+3)-9)+72)//144
	return (n*n*(n+3)+72)//144

@lru_cache(maxsize=None)
def prim(n):
	return triag(n) - sum(prim(n//k) for k in range(2, n//3 + 1))

def prim_brute(n):
	res = 0
	for a in range(1, n//3 + 1):
		for b in range(a, n//2 + 1):
			m = gcd(a, b)
			for c in range(b, min(a + b, n - a - b + 1)):
				if gcd(m, c) == 1:
					res += 1
	return res

"""
n = 0
while True:
	comp_res = prim(n)
	true_res = prim_brute(n)
	if comp_res != true_res:
		print("Mismatch detected!")
		break
	print("prim({})={}".format(n, comp_res))
	n += 1
"""
n = 10000000
print(prim(n))

