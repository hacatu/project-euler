class Ant:
	def __init__(self):
		self.black_cells = set()
		self.ant_pos = (128, 128)
		self.ant_dir = (1, 0)
		self.next_dir = {
			(1, 0, 0): (0, -1), (0, -1, 0): (-1, 0), (-1, 0, 0): (0, 1), (0, 1, 0): (1, 0),
			(1, 0, 1): (0, 1), (0, 1, 1): (-1, 0), (-1, 0, 1): (0, -1), (0, -1, 1): (1, 0)
		}
		self.turn = 0

	def update(self):
		x, y = self.ant_pos
		if self.ant_pos in self.black_cells:
			self.black_cells.remove(self.ant_pos)
			self.ant_dir = self.next_dir[self.ant_dir + (1,)]
		else:
			self.black_cells.add(self.ant_pos)
			self.ant_dir = self.next_dir[self.ant_dir + (0,)]
		dx, dy = self.ant_dir
		self.ant_pos = (x + dx, y + dy)
		self.turn += 1

a = [0]*10081
ant = Ant()
while ant.turn < 10080:
	ant.update()
	a[ant.turn] = len(ant.black_cells)

