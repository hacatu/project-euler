from functools import cache
import math

@cache
def Ainv(t, g):
	if t == 0:
		return 1/2**g
	if g == 0:
		return 1
	k = min(t, g)
	return sum(math.comb(k, i)*Ainv(t - k + i, g - i) for i in range(k + 1))/2**k

def Ainv_cf(n):
	return sum((1+2**(n-i))*math.comb(n-1+i, n-1)/2**(2*n) for i in range(n))

print(1/Ainv_cf(10000))

