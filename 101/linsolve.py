def linsolve(eqns):
	n, m = len(eqns), len(eqns[0])
	for c in range(n):
		if eqns[c][c] == 0:
			for s in range(c + 1, n):
				if eqns[s][c]:
					eqns[c], eqns[s] = eqns[s], eqns[c]
					break
			else:
				return None
		for j in range(c, m)[::-1]:
			eqns[c][j] /= eqns[c][c]
		for i in range(n):
			if i == c:
				continue
			for j in range(c, m)[::-1]:
				eqns[i][j] -= eqns[c][j]*eqns[i][c]
	return [row[m - 1] for row in eqns]

def pval(ns, x):
	return sum([n*x**i for (i, n) in enumerate(ns)])

