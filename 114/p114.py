"""If you use recursion, all of your problems will go away.
 * red-ended blocks: reb(1) = reb(2) = 0; reb(3) = 1
 * reb(n) = E(reb(i);i,j;i+j<n)+1
 * 
 * blocks: b(n) = E(reb(n-i-j);i+j<n)+1
"""

MAX = 50

_reb = [0]*(MAX + 1)
_reb[3] = 1
_reb[4] = 1
_reb[5] = 1
_reb[6] = 1

def reb(n):
	if _reb[n] == 0:
		c = 1
		for i in range(3, n - 3):
			c += reb(i)*(n - 3 - i)
		_reb[n] = c
	return _reb[n]

def b(n):
	c = 1
	for i in range(0, n - 2):
		c += reb(n - i)*(i + 1)
	return c

print(b(MAX))

