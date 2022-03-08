from math import sqrt

class Tiling:
	def __init__(self, n, k, a, b, is_tall):
		self.n = n
		self.k = k
		self.a = a
		self.b = b
		d = k*k
		d = n*sqrt(n*n - 12*(d*d)*(a*a)*(b*b))
		if is_tall:
			self.w = sqrt((n*n-d)/2)/(2*k*a)
			self.h = sqrt((n*n+d)/2)/(2*k*b)
		else:
			self.w = sqrt((n*n+d)/2)/(2*k*a)
			self.h = sqrt((n*n-d)/2)/(2*k*b)