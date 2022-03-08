from math import sin, pi
#from matplotlib import pyplot as plt

#import sys, os
#sys.path.insert(0, '/usr/lib/python3.8/site-packages')
#sys.path.insert(0, os.path.expanduser('~/.local/lib/python3.8/site-packages'))

from sortedcontainers import SortedDict
#import sympy

sin_table = [sin(pi*t/180) for t in range(0, 181)]

class Triangle:
	def __init__(self, A, B, C):
		self.A = A
		self.B = B
		self.C = C
		self.p = sin_table[A + C]*sin_table[B]
        self.q = sin_table[C - B]*sin_table[A]

#triangles = []

ratio_groups = SortedDict()

for C in range(2, 91):
	print("C: {}".format(C))
	for A in range(1, 90):
		for B in range(1, C):
			T = Triangle(A, B, C)
			ratio_groups.setdefault(T.ratio, set()).add(T)

#triangles.sort()
#deltas = [(triangles[i + 1][0] - triangles[i][0], i) for i in range(len(triangles) - 1)]
#deltas.sort()

#m = int(len(deltas)*0.1)

#deltas = deltas[5*m:6*m]
#m//4 m//3
#135282 is the last zero
#135283 is 8.673617379884035e-19

#print("{} to {}".format(deltas[0][0], deltas[-1][0]))

#plt.scatter(range(m), [d for (d, i) in deltas])
#plt.show()

print("{} unique ratios (out of {} triples of angles)".format(len(ratio_groups), 89*89*45))

