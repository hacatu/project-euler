from frozendict import frozendict
import itertools as itt
from collections import Counter

M, pM = 23, 22
N = 23

factorial = [0]*24
factorial[0] = 1
for i in range(1, 24):
	factorial[i] = factorial[i-1]*i
factorial = tuple(factorial)

def next_ways(ways, n, m):
	n_ways = [0]*M
	for e in range(pM):
		for r, w in enumerate(ways):
			n_ways[(r + m*10**e)%M] += (n-e+pM-1)//pM*w
	return n_ways

def partitions(n, m=0, pre={}):
	if n == 0:
		yield frozendict(pre)
		return
	if m == 0:
		m = n
	if m == 1:
		yield frozendict(pre | {1: n})
		return
	for k in range(n//m + 1):
		if k:
			n_pre = pre | {m: k}
		else:
			n_pre = pre
		yield from partitions(n - k*m, m - 1, n_pre)

def o_ways(n, p):
	ways = [1] + [0]*(M-1)
	for m, k in p.items():
		for _ in range(k):
			ways = next_ways(ways, n, m)
	return ways

class PartitionTreeNode:
	def __init__(self, p, v):
		self.p = p
		self.iov = v
		self.children = Counter()
	
	def populateDescendents(self, tree):
		self.descendents = Counter(self.children)
		for p, m in self.children.items():
			node = tree.map[p]
			for d, c in node.descendents.items():
				self.descendents[d] += c*m

class PartitionTree:
	def __init__(self, n):
		self.map = {}
		p = frozendict({1:23})
		self.map[p] = PartitionTreeNode(p, sum(o_ways(n, p)))
		self.rows = [{p}]
		while len(self.rows) < 23:
			print("Building partitions with {} parts".format(23 - len(self.rows)))
			self.rows.append(set())
			for p in self.rows[-2]:
				node = self.map[p]
				for (ma, ka), (mb, kb) in itt.combinations_with_replacement(p.items(), 2):
					mc = ma + mb
					p2 = dict(p)
					if ma == mb:
						if ka == 1:
							continue
						elif ka == 2:
							del p2[ma]
						else:
							p2[ma] -= 2
					else:
						if ka == 1:
							del p2[ma]
						else:
							p2[ma] -= 1
						if kb == 1:
							del p2[mb]
						else:
							p2[mb] -= 1
					if mc in p2:
						p2[mc] += 1
					else:
						p2[mc] = 1
					p2 = frozendict(p2)
					l = len(self.rows[-1])
					self.rows[-1].add(p2)
					if len(self.rows[-1]) != l:
						self.map[p2] = PartitionTreeNode(p2, sum(o_ways(n, p2)))
					if ma == mb:
						node.children[p2] += ka*(ka - 1)//2
					else:
						node.children[p2] += ka*kb
		self.rows = self.rows[::-1]
	
	def computeExclusiveOrderedVs(self):
		for i, row in enumerate(self.rows):
			print("Removing descendent eovs from partitions with {} parts".format(i + 1))
			for p in row:
				node = self.map[p]
				node.populateDescendents(self)
				node.eov = node.iov
				for d, m in node.descendents.items():
					node.eov -= self.map[d].eov*m
			if i:
				for p in self.rows[i-1]:
					del self.map[p].descendents
		for p in self.rows[-1]:
			del self.map[p].descendents
	
	def computeExclusiveUnorderedVs(self):
		for p, node in self.map.items():
			node.euv = node.eov
			d = 1
			for _, k in p.items():
				if k != 1:
					d *= factorial[k]
			if d != 1:
				node.euv //= d

ptt = PartitionTree(23)
ptt.computeExclusiveOrderedVs()
ptt.computeExclusiveUnorderedVs()

