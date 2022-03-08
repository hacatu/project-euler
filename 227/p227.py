import numpy as np
from random import randint


n = 50
#M = np.matrix([[5/9, 0], [4/9, 1]])
M = np.matrix([[1/2, 2/9, 0], [4/9, 5/9, 0], [1/18, 2/9, 1]])
#M = np.matrix([[1/2, 2/9, 1/36] + [0]*(n - 2), [4/9, 19/36, 2/9, 1/36 if n > 3 else 0] + [0]*(n - 3)] + [([0]*(n - 2 - k) + [1/36, 2/9, 1/2, 2/9, 1/36 if k > 2 else 0] + [0]*(k + 3))[:n + 1] for k in range(n - 2, 1, -1)] + [[0]*(n - 3) + [1/36, 2/9, 19/36, 0], [0]*(n - 2) + [1/36, 2/9, 1]])
#print(M)
M_i = M
M_i_1 = np.identity(M.shape[0])

"""
s = 0
ps = [0]*59999
for i in range(1, 60000):
	ps[i - 1] = i*(M_i - M_i_1)[-1, 0]
	M_i, M_i_1 = M @ M_i, M_i
print(sum(ps[::-1]))
"""

def empirical(n, trials):
	s = 0
	for _ in range(trials):
		a, b = 0, n
		c = 0
		while a != b:
			d = randint(1, 6)
			if d == 1:
				a = (a - 1)%(2*n)
			elif d == 6:
				a = (a + 1)%(2*n)
			d = randint(1, 6)
			if d == 1:
				b = (b - 1)%(2*n)
			elif d == 6:
				b = (b + 1)%(2*n)
			c += 1
		s += c
	return s/trials

"""
E = [empirical(n, 100) for n in range(3, 51)]

with open("En.csv", "w") as f:
	for n, e in enumerate(E, 3):
		f.write("{},{}\n".format(n, e))
"""

