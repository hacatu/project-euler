from collections import defaultdict

def build_chain_ptable(F, table, support):
	return {f: sum(F(x, f)*p for x, p in table.items()) for f in support}

class DieDistribution(dict):
	def __init__(self, s):
		self.s = s
		self[1] = defaultdict(int, {x: 1/s for x in range(1, s + 1)})
		self.max = 1
	
	def __missing__(self, n):
		for k in range(self.max + 1, n + 1):
			self[k] = defaultdict(int, {m: sum(self[k - 1][m - i] for i in range(1, m))/self.s for m in range(k, k*self.s + 1)})
		return self[n]
	
	def support(self, n):
		return range(1, self.s*n + 1)

dieDistributions = {s: DieDistribution(s) for s in (4, 6, 8, 12, 20)}

table1 = {1: 1}
table2 = build_chain_ptable(lambda x, f: dieDistributions[4][x][f], table1, dieDistributions[4].support(1))
table3 = build_chain_ptable(lambda x, f: dieDistributions[6][x][f], table2, dieDistributions[6].support(4))
table4 = build_chain_ptable(lambda x, f: dieDistributions[8][x][f], table3, dieDistributions[8].support(24))
table5 = build_chain_ptable(lambda x, f: dieDistributions[12][x][f], table4, dieDistributions[12].support(192))
#table6 = build_chain_ptable(lambda x, f: dieDistributions[20][x][f], table5, dieDistributions[20].support(2304))

print(table3)

#print(sum(x**2*p for x, p in table2.items()) - sum(x*p for x, p in table2.items())**2)

