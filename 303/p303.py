import sys
import itertools as itt

def is_small(n):
	while n:
		if n%10 >= 3:
			return False
		n //= 10
	return True

def fst(t):
	return t[0]

def snd(t):
	return t[1]

class Node:
	def __init__(self, label):
		self.label = label
		self.is_small = is_small(label)
		self.visited = False
		self.ins = set()
		self.outs = {}

class Mulgraph:
	def __init__(self, n):
		self.nodes = {}
		frontier = {0}
		next_frontier = set()
		min_d = 1
		self.candidate_tails = set()
		self.min_k = None
		while True:
			if not frontier:
				if self.candidate_tails or not next_frontier:
					break
				if r == 0:
					min_d = 0
				frontier = next_frontier
				next_frontier = set()
			r = frontier.pop()
			if r not in self.nodes:
				self.nodes[r] = Node(r)
			node = self.nodes[r]
			node.visited = True
			#print(f"Visiting node {r}")
			for d in range(min_d, 10):
				m = n*d + r
				if m%10 < 3:
					m //= 10
					if m in frontier:
						continue
					if m not in self.nodes:
						self.nodes[m] = Node(m)
					elif self.nodes[m].visited:
						continue
					#print(f" -> {m}")
					node.outs[d] = m
					self.nodes[m].ins.add((r, d))
					if is_small(m) and d:
						self.candidate_tails.add(m)
						break
					next_frontier.add(m)
		#print(f"Found {len(self.nodes)} nodes")
	
	def prune(self):
		frontier = set()
		for node in self.nodes.values():
			if not node.is_small and not node.outs:
				frontier.add(node)
		while frontier:
			node = frontier.pop()
			#print(f"Removing node {node.label}")
			del self.nodes[node.label]
			for r, d in node.ins:
				#print(f" from {r}")
				other = self.nodes[r]
				del other.outs[d]
				if not other.outs:
					frontier.add(other)
	
	def calc_min_k(self):
		def calc_dm(m):
			return min(d for (_, d) in self.nodes[m].ins), m
		def calc_tail_k(m):
			if not m:
				return 0, 0
			k = 0
			min_d, _ = calc_dm(m)
			k, d = min(calc_tail_k(r) for (r, d) in self.nodes[m].ins if d == min_d)
			return k + min_d*10**d, d + 1
		if self.min_k is not None:
			return self.min_k
		if len(self.candidate_tails) == 1:
			self.min_k, _ = calc_tail_k(self.candidate_tails.pop())
			return self.min_k
		inlets = map(calc_dm, self.candidate_tails)
		tails = next(itt.groupby(sorted(inlets), fst))[1]
		self.min_k, _ = min(calc_tail_k(m) for (_, m) in tails)
		return self.min_k
	
	def print(self):
		print("digraph {")
		print(" overlap=false")
		print(" splines=true")
		for r, node in self.nodes.items():
			if node.is_small:
				print(f" {r} [color=green];")
			else:
				print(f" {r};")
			for d, m in node.outs.items():
				print(f"   {r} -> {m} [label={d}];")
		print("}")

s = 2
for n in range(3, 10001):
	graph = Mulgraph(n)
	k = graph.calc_min_k()
	#print(f"{n}: {k}")
	s += k
print(s)

