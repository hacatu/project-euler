from math import gcd
from copy import copy

class CompositeModulusException(Exception):
	def __init__(self, d):
		self.d = d

class EC_Point:
	def __init__(self, ctx, x, y, is_id):
		self.ctx = ctx
		self.x = x
		self.y = y
		self.is_id = is_id
	
	def __str__(self):
		if self.is_id:
			return "id on a={} mod {}".format(self.ctx.a, self.ctx.n)
		return "({}, {}) on a={} mod {}".format(self.x, self.y, self.ctx.a, self.ctx.n)
	
	def __repr__(self):
		return "EC_Point(EC({}, {}, {}), {}, {}, {})".format(self.ctx.a, self.ctx.b, self.ctx.n, self.x, self.y, self.is_id)
	
	def __add__(self, other):
		if self.is_id:
			if other.is_id:
				return EC_Point(self.ctx, 0, 0, True)
			return EC_Point(self.ctx, other.x, other.y, False)
		if other.is_id:
			return EC_Point(self.ctx, self.x, self.y, False)
		if self.x != other.x:
			dy = (self.y - other.y)%self.ctx.n
			dx = (self.x - other.x)%self.ctx.n
		elif self.y != other.y or self.y == 0:
			return EC_Point(self.ctx, 0, 0, True)
		else:
			dy = (3*self.x*self.x + self.ctx.a)%self.ctx.n
			dx = (2*self.y)%self.ctx.n
		try:
			s = pow(dx, -1, self.ctx.n)
		except ValueError:
			raise CompositeModulusException(gcd(dx, self.ctx.n))
		s = pow(dx, -1, self.ctx.n)
		s = s*dy%self.ctx.n
		x = (s*s - self.x - other.x)%self.ctx.n
		y = (self.y + s*(x - self.x))%self.ctx.n
		return EC_Point(self.ctx, x, y, False)
	
	def double(self):
		if self.is_id or self.y == 0:
			return EC_Point(self.ctx, 0, 0, True)
		dy = (3*self.x*self.x + self.ctx.a)%self.ctx.n
		dx = (2*self.y)%self.ctx.n
		try:
			s = pow(dx, -1, self.ctx.n)
		except ValueError:
			raise CompositeModulusException(gcd(dx, self.ctx.n))
		s = s*dy%self.ctx.n
		x = (s*s - 2*self.x)%self.ctx.n
		y = (self.y + s*(x - self.x))%self.ctx.n
		return EC_Point(self.ctx, x, y, False)
	
	def __mul__(self, k):
		if self.is_id or k == 0:
			return EC_Point(self.ctx, 0, 0, True)
		s = copy(self)
		while (k&1) == 0:
			s = s.double()
			if s.is_id:
				return s
			k >>= 1
		r = copy(s)
		while True:
			k >>= 1
			if k == 0:
				return r
			s = s.double()
			if s.is_id:
				return r
			if k&1:
				r = r + s

class EC:
	def __init__(self, a, b, n):
		self.a = a
		self.b = b
		self.n = n
	
	@classmethod
	def makePointOnCurve(cls, x, y, a, n):
		b = (x*x + a)%n
		b = (y*y - x*b)%n
		ctx = EC(a, b, n)
		return EC_Point(ctx, x, y, False)

