import pygame
import sys

SCREEN_WIDTH = 1200

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_WIDTH))
pygame.display.set_caption("Project Euler 349")

CELLS_PER_SIDE = 256

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
			pygame.draw.rect(screen, 0xFFFFFFFF, (x*SCREEN_WIDTH/CELLS_PER_SIDE, y*SCREEN_WIDTH/CELLS_PER_SIDE, SCREEN_WIDTH/CELLS_PER_SIDE, SCREEN_WIDTH/CELLS_PER_SIDE))
		else:
			self.black_cells.add(self.ant_pos)
			self.ant_dir = self.next_dir[self.ant_dir + (0,)]
			pygame.draw.rect(screen, 0xFF000000, (x*SCREEN_WIDTH/CELLS_PER_SIDE, y*SCREEN_WIDTH/CELLS_PER_SIDE, SCREEN_WIDTH/CELLS_PER_SIDE, SCREEN_WIDTH/CELLS_PER_SIDE))
		dx, dy = self.ant_dir
		self.ant_pos = (x + dx, y + dy)
		self.turn += 1
		print("turn: {} black_cells: {}".format(self.turn, len(self.black_cells)))

clock = pygame.time.Clock()
ant = Ant()

screen.fill(0xFFFFFFFF)
while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
			pygame.quit()
			sys.exit(0)
		if e.type == pygame.KEYDOWN and e.key == pygame.K_RIGHT:
			pass
	ant.update()
	clock.tick(60)
	pygame.display.flip()
	if ant.turn == 10081:
		pygame.quit()
		sys.exit()

