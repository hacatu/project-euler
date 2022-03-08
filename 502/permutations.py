import numpy as np
from bitarray import bitarray
import copy

h = 3

def cycles(pmat):
	visited = bitarray(pmat.shape[0])
	visited.setall(False)
	res = []
	while True:
		try:
			i = visited.index(0)
			res.append([i])
			while True:
				visited[i] = 1
				for j, row in enumerate(pmat):
					if row[i]:
						i = j
						break
				if i == res[-1][0]:
					break
				res[-1].append(i)
		except ValueError:
			return res

def pmat(perm):
	res = [[0]*len(perm) for _ in perm]
	for i, j in enumerate(perm):
		res[j][i] = 1
	return np.array(res, dtype=np.int64)

#dW_4 = P_(0,1,2,3,4,5,6,7) + P_(2,3,0,1,6,7,4,5) + P_(7,0,1,2,3,4,5,6) + P_(5,6,7,0,1,2,3,4)
#dW_4 = P_(0,1,2,3,4,5,6,7) + P_(7,6,1,0,3,2,5,4) + P_(7,0,1,2,3,4,5,6) + P_(5,0,7,2,1,4,3,6)
def print_mat_group(h):
	for s in range(2, h, 2):
		print("Subset generated by permutation matrix {}".format(s))
		P = pmat((5,0,7,2,1,4,3,6))
		S = P
		while True:
			print(S)
			#print(cycles(S))
			S = S@P
			if np.array_equal(S, P):
				break

class Permutation:
	def __init__(self, js):
		self.js = js
	
	def __imul__(self, other):
		self.js = tuple(self.js[j] for j in other.js)
		return self
	
	def __mul__(self, other):
		res = copy.copy(self)
		res.__imul__(other)
		return res
	
	def __ipow__(self, n):
		s = copy.copy(self)
		if not (n&1):
			self.js = tuple(range(len(self.js)))
		while n := n >> 1:
			s *= s
			if n&1:
				self *= s
		return self
	
	def __pow__(self, n):
		res = copy.copy(self)
		res.__ipow__(n)
		return res
	
	def __len__(self):
		return len(self.js)
	
	def __eq__(self, other):
		return self.js == other.js
	
	def __ne__(self, other):
		return self.js != other.js
	
	def __hash__(self):
		return hash(self.js)
	
	def __str__(self):
		return str(self.js)
	
	def __repr__(self):
		return "Permutation({})".format(self.js)

def iterate(perm):
	it = perm
	while True:
		print(it)
		it = tuple(perm[j] for j in it)
		if it == perm:
			break
