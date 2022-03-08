import numpy as np
from functools import reduce
from math import floor

"""
x1 + x2 + ... + xn = n
x1 = n - x2 - x3 - ... - xn
x1^1*x2^2*...*xn^n = max
(n - x2 - x3 - ... - xn)^1*x2^2*...*xn^n = max
1*ln(n - x2 - x3 - ... xn) + 2*ln x2 + ... + n*ln xn = max
d/dxj: j/xj = 1/(n - x2 - ... - xn)
xj = j*(n - x2 - ... - xn)
j*n = j*x2 + ... + j*x[j - 1] + (j + 1)*xj + j*x[j + 1] + ... + j*xn
"""

for n in range(2, 16):
	a = np.array([[i if j != i else (i + 1) for j in range(2, n + 1)] for i in range(2, n + 1)])
	print(a)
	b = np.array([i*n for i in range(2, n + 1)])
	x = np.linalg.solve(a, b)
	x = [reduce(lambda l, e: l - e, x, n)] + list(x)
	print(n, floor(reduce(lambda l, t: l + t[1]**t[0], enumerate(x, 1), 0)))
	print(x)

