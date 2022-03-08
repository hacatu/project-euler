from fractions import Fraction as Q
from itertools import combinations
import heapq
import bisect
from copy import deepcopy

def copy_class(cls, new_name):
	new_cls = type(new_name, cls.__bases__, deepcopy(A.__dict__))
	new_cls.__name__ = new_name
	return new_cls

def DeriveCmpFromOrd(cls):
	cls.__lt__ = def __lt__(self, other):
		return self.ord(other) < 0
	cls.__le__ = def __le__(self, other):
		return self.ord(other) <= 0
	cls.__eq__ = def __eq__(self, other):
		return self.ord(other) == 0
	cls.__ne__ = def __ne__(self, other):
		return self.ord(other) != 0
	cls.__ge__ = def __ge__(self, other):
		return self.ord(other) >= 0
	cls.__gt__ = def __gt__(self, other):
		return self.ord(other) > 0
	return cls

def DeriveReverseCmpFromOrd(cls):
	cls.__lt__ = def __lt__(self, other):
		return self.ord(other) > 0
	cls.__le__ = def __le__(self, other):
		return self.ord(other) >= 0
	cls.__eq__ = def __eq__(self, other):
		return self.ord(other) == 0
	cls.__ne__ = def __ne__(self, other):
		return self.ord(other) != 0
	cls.__ge__ = def __ge__(self, other):
		return self.ord(other) <= 0
	cls.__gt__ = def __gt__(self, other):
		return self.ord(other) < 0
	return cls

class DDLine:
	def __init__(self, p1, p2):
		self.m = None if p1[0] == p2[0] else Q(p2[1] - p1[1], p2[0] - p1[0])
		self.b = (Q(p1[0]), Q(0)) if p1[0] == p2[0] else (Q(0), p1[1] - self.m*p1[0])
	
	def __eq__(self, other):
		if not isinstance(other, DDLine):
			return False
		return self.m == other.m and self.b == other.b
	
	def __hash__(self):
		return hash((self.m, self.b))

def T_gen():
	s = 290797
	while True:
		s = s*s%50515093
		yield s%2000 - 1000

def points_gen():
	T = T_gen()
	while True:
		yield (next(T), next(T))

points = points_gen()
points = list(set(next(points) for _ in range(100)))

lines = list({DDLine(a, b) for (a, b) in combinations(points, 2)})

@DeriveCmpFromOrd
class Line:
	def __init__(self, p, m):
		self.p = p
		self.m = m
	
	def y(self, x):
		return m*(x - self.p[0]) + self.p[1]
	
	def renormalize(self, x):
		self.p = (x, self.y(x))
		return self
	
	def intersect(self, other):
		if self.m == other.m:
			return None
		x = (self.m*self.p[0] - other.m*other.p[0] + other.p[1] - self.p[1])/(self.m - other.m)
		return (x, self.y(x))
	
	def ord(self, other):
		if self.p[1] == other.p[1]:
			if self.m == other.m:
				return 0
			return -1 if self.m < other.m else 1
		return -1 if self.p[1] < other.p[1] else 1
	
	def __hash__(self):
		return hash((self.p, self.m))

@DeriveCmpFromOrd
class Event:
	def __init__(self, p):
		self.p = p
	
	def ord(self, other):
		if self.p[0] != other.p[0]:
			return -1 if self.p[0] < other.p[0] else 1
		elif isinstance(self, VerticalEvent):
			return -1
		elif isinstance(other, VerticalEvent):
			return 1
		return -1 if self.p[1] < other.p[1] else 1
	
	def __hash__(self):
		return hash(self.p)

class IntersectEvent(Event):
	def __init__(self, p, *lines):
		super().__init__(self, p)
		self.lines = lines
	
	@classmethod
	def make(cls, l1, l2):
		p = l1.intersect(l1)
		if p is None:
			return None
		return IntersectEvent(p, l1, l2)

ReverseIntersectEvent = DeriveReverseCmpFromOrd(copy_class(IntersectEvent))

class VerticalEvent(Event):
	def __init__(self, x):
		super().__init__(self, (x, 0))

ReverseVerticalEvent = DeriveReverseCmpFromOrd(copy_class(VerticalEvent))

class Scanline:
	def __init__(self, lines, x):
		self.verticals = []
		self.intersections = []
		for line in lines:
			if line.m is None:
				self.verticals.append[line.x]
			else:
				self.intersections.append(Line(line.b, line.m))
		self.verticals.sort()
		self.intersections.sort()
		self.events = []
		self.counter = 2*len(self.verticals)*len(self.intersections)
	
	def beginScanRight(self):
		for i in range(0, len(self.intersections) - 1):
			intersection = IntersectEvent.make(self.intersections[i], self.intersections[i + 1])
			if intersection is not None:
				self.events.append(intersection)
		heapq.heapify(self.events)
	
	def scanRight(self):
		while self.events:
			ev = heapq.pop(self.events)
			if isinstance(ev, IntersectEvent):
				lines = set(ev.lines)
				while self.events and isinstance(self.events[0], IntersectEvent) and self.events[0].p == ev.p:
					lines |= self.events[0].lines
					heapq.pop(self.events)
				counter += len(lines)*(len(lines) - 1)

print(len(lines))

