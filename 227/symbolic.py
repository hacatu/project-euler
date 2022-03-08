import sympy

def get_M(n):
	if n == 1:
		return sympy.Matrix([[5, 0], [4, 9]])/9
	elif n == 2:
		return sympy.Matrix([[9, 4, 0], [8, 10, 0], [1, 4, 18]])/18
	n_col = [18, 16, 2] + [0]*(n - 2)
	n_1_col = [8, 19, 8, 1] + [0]*(n - 3)
	z_col = [0]*(n) + [36]
	o_col = [0]*(n - 3) + [1, 8, 19, 8]
	if n > 3:
		k_col = [0]*(n - 4) + [1, 8, 18, 8, 1] + [0]*(n - 4)
	k_cols = [k_col[k - 2:n + k - 1] for k in range(n - 2, 1, -1)]
	cols = [n_col, n_1_col] + k_cols + [o_col, z_col]
	return sympy.Matrix(cols).T/36

def compute_E(n):
	M = get_M(n)
	S, J = M.diagonalize()
	fJ = sympy.diag(0, *((1-J[i,i])**-2 for i in range(1, n + 1)))
	fJ = sympy.expand(fJ)
	fJ = sympy.simplify(fJ)
	L = sympy.Matrix([[0]*n + [1]]) @ (M - sympy.eye(n + 1)) @ S
	L = sympy.simplify(L)
	R = S**-1 @ sympy.Matrix([[1]] + [[0]]*n)
	R = sympy.simplify(R)
	E = (L @ fJ @ R)[-1,0]
	return sympy.simplify(E)
	

"""
E(1) = 9/4
E(2) = 36/5
E(3) = 675/44
E(4) = 
"""

