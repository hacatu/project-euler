#from fractions import Fraction as Q
def Q(a, b=1):
	return a/b

class PythTree:
	T_L = [[Q(16, 25), -Q(12, 25), Q(0)], [Q(12, 25), Q(16, 25), Q(1)], [Q(0), Q(0), Q(1)]]
	T_R = [[Q(9, 25), Q(12, 25), Q(16, 25)], [Q(-12, 25), Q(9, 25), Q(37, 25)], [Q(0), Q(0), Q(1)]]
	
	def __init__(self):
		self.squares = [((Q(0), Q(0)), (Q(1), Q(0)), (Q(1), Q(1)), (Q(0), Q(1)))]
		self.bottom = Q(0)
		self.top = Q(1)
		self.left = Q(0)
		self.right = Q(1)
		self.tolerance = Q(8)#5*sqrt(2)

	def update(self):
		tmp_squares = []
		for T in (PythTree.T_L, PythTree.T_R):
			for _square in self.squares:
				square = tuple((T[0][0]*x + T[0][1]*y + T[0][2], T[1][0]*x + T[1][1]*y + T[1][2]) for (x, y) in _square)
				_, _, (x1, y1), (x2, y2) = square
				near_boundary = False
				c_min, c_max = sorted((x1, x2))
				if c_min < self.left:
					self.left = c_min
					near_boundary = True
				elif c_max > self.right:
					self.right = c_max
					near_boundary = True
				elif c_min - self.tolerance <= self.left:
					near_bounrary = True
				elif c_max + self.tolerance >= self.right:
					near_bondary = True
				c_min, c_max = sorted((y1, y2))
				if c_min < self.bottom:
					self.bottom = c_min
					near_boundary = True
				elif c_max > self.top:
					self.top = c_max
					near_boundry = True
				if near_boundary or c_min - self.tolerance <= self.bottom or c_max + self.tolerance >= self.top:
					tmp_squares.append(square)
		self.squares = tmp_squares
		#print(self.squares)
		self.tolerance *= Q(4, 5)
	
	def draw(self):
		a_min = float(self.top - self.bottom)*float(self.right - self.left)
		a_max = float(self.top - self.bottom + 2*self.tolerance)*float(self.right - self.left + 2*self.tolerance)
		print("Boundary Squares: {}".format(len(self.squares)))
		print("Tolerance: {}".format(float(self.tolerance)))
		print("Left:      {}".format(float(self.left)))
		print("Right:     {}".format(float(self.right)))
		print("Top:       {}".format(float(self.top)))
		print("Bottom:    {}".format(float(self.bottom)))
		print("Area:      {} to {}".format(a_min, a_max))

tree = PythTree()

tree.draw()
while True:
	input()
	tree.update()
	tree.draw()

