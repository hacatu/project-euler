from fractions import Fraction as Q
from itertools import combinations
from collections import Counter

class Line:
	def __init__(self, p1, p2):
		self.m = None if p1[0] == p2[0] else Q(p2[1] - p1[1], p2[0] - p1[0])
		self.b = (Q(p1[0]), Q(0)) if p1[0] == p2[0] else (Q(0), p1[1] - self.m*p1[0])
	
	def __eq__(self, other):
		if not isinstance(other, Line):
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
points = list(set(next(points) for _ in range(2500)))

lines = list({Line(a, b) for (a, b) in combinations(points, 2)})
L = len(lines)

linesBySlope = Counter(line.m for line in lines)
S = sum(count*(L - count) for count in linesBySlope.values())

print(L)
print(S)

