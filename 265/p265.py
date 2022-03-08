class DeBruijnIterator:
	def __init__(self, n):
		self.n = n
		self.s = [0]*(1 << n)
		self.l = [1] + [0]*((1 << n) - 1)
		self.t = 0
		self.j = n
	
	def complete(self):
		if self.j == 1 << self.n:
			return False
		while self.j != 1 << self.n:
			t = (self.t << 1) | 1
			t &= ((1 << self.n) - 1)
			if not self.l[t]:
				self.s[self.j] = 1
			elif not self.l[t - 1]:
				self.s[self.j] = 0
				t -= 1
			else:
				return False
			self.t = t
			self.l[self.t] = 1
			self.j += 1
		return True
	
	def backtrack(self):
		print("backtracking from {}".format(self.j))
		print("sequence is {}".format(self.s))
		print("tail is {}".format(self.t))
		while True:
			self.j -= 1
			print("inspecting index {} (will shift {} onto tail)".format(self.j, self.j - self.n))
			if self.j < self.n:
				return False
			self.l[self.t] = 0
			self.t >>= 1
			self.t |= self.s[self.j - self.n] << (self.n - 1)
			print("tail is {}".format(self.t))
			if self.s[self.j]:
				print("backtracked to {}".format(self.j))
				return True
	
	def __iter__(self):
		def sequence_generator():
			self.complete()
			print("first sequence: {}".format(self.s))
			yield self.s
			self.s = self.s[self.n:] + [0]*self.n
			self.t = 0
			self.j = 1 << self.n
			self.l[0] = 0
			self.l[1] = 0
			self.j = (1 << self.n) - self.n
			while True:
				if not self.backtrack():
					return
				if self.complete():
					yield self.s
		return sequence_generator()

def S(n):
	r = 0
	for s in DeBruijnIterator(n):
		z = 0
		for i, b in enumerate(s):
			if not b:
				z += 1
			else:
				z = 0
			if z == n:
				s = s[i + 1:] + s[:i - n + 1]
				break
		print("sequence besides zeroes: {}".format(s))
		t = 0
		for b in s:
			t = (t << 1) | b
		r += t
	return r

S(2)

