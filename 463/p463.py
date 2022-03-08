from functools import lru_cache
import sys
sys.setrecursionlimit(10000)

@lru_cache(maxsize=None)
def S(n):
	if n < len(S_table):
		return S_table[n]
	if n&3 == 0:
		return (S(n//2) + 5*S(n//2 - 1) - 8*S(n//4 - 1) - 1)#%MOD
	elif n&3 == 1:
		return (S(n//2) + 2*S(n//2 + 1) - 3*S(n//4) + 3*S(n//2 - 1) - 5*S(n//4 - 1) - 1)#%MOD
	elif n&3 == 2:
		return (3*S(n//2) - 3*S(n//4) + 3*S(n//2 - 2) - 5*S(n//4 - 1) - 1)#%MOD
	return (S(n//2) + 2*S((n-1)//4*2 + 1) - 3*S((n-1)//4) + 3*S((n-3)//4*2 + 1) - 5*S((n-3)//4) - 1)#%MOD

f_table = [None]*3**15
S_table = [None]*3**15

def f(n):
	if n < 2:
		return n
	if n&1 == 0:
		return f_table[n >> 1]
	if n&2:
		return 3*f_table[n >> 1] - 2*f_table[n >> 2]
	return 2*f_table[(n >> 1) + 1] - f_table[n >> 2]

for n in range(len(f_table)):
	f_table[n] = f(n)

S_table[0] = 0

for n in range(1, len(f_table)):
	S_table[n] = S_table[n - 1] + f_table[n]

del f_table

for p in range(38):
	print("S(3**{}) = {}".format(p, S(3**p)))

