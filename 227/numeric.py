import numpy as np
import numpy.linalg as lin

def get_M(n):
	if n == 1:
		return np.array([[5/9, 0], [4/9, 1]])
	elif n == 2:
		return np.array([[1/2, 2/9, 0], [4/9, 5/9, 0], [1/18, 2/9, 1]])
	n_col = [1/2, 4/9, 1/18] + [0]*(n - 2)
	n_1_col = [2/9, 19/36, 2/9, 1/36] + [0]*(n - 3)
	z_col = [0]*(n) + [1]
	o_col = [0]*(n - 3) + [1/36, 2/9, 19/36, 2/9]
	if n > 3:
		k_col = [0]*(n - 4) + [1/36, 2/9, 1/2, 2/9, 1/36] + [0]*(n - 4)
	k_cols = [k_col[k - 2:n + k - 1] for k in range(n - 2, 1, -1)]
	cols = [n_col, n_1_col] + k_cols + [o_col, z_col]
	return np.array(cols).transpose()

def compute_E(n):
	M = get_M(n)
	j, S = lin.eig(M)
	fJ = np.diag([0] + [(1-j[i])**-2 for i in range(1, n + 1)])
	L = (M - np.eye(n + 1))[-1] @ S
	R = lin.inv(S)[:,0]
	return L @ fJ @ R

print(compute_E(50))

