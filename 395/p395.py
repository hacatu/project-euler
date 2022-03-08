import pygame
import sys
import cmath
import numpy as np

SCREEN_WIDTH = 1200

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_WIDTH))
pygame.display.set_caption("Broccoli")

def to_screen(z):
	z = SCREEN_WIDTH/9*(z + 4 + 1J)
	return (z.real, z.imag)

class PythTree:
	T_L = np.array([[16/25 + 12J/25, 1J], [0, 1]], dtype=np.complex_)
	T_R = np.array([[9/25 - 12J/25, 16/25 + 37J/25], [0, 1]], dtype=np.complex_)
	
	def __init__(self):
		self.squares = [(0, 1, 1+1J, 1J)]
		self.bottom = 0
		self.top = 1
		self.left = 0
		self.right = 1
		self.tolerance = 5*np.sqrt(2)

	def update(self):
		self.squares = [tuple(T[0,0]*z + T[0,1] for z in square) for T in (PythTree.T_L, PythTree.T_R) for square in self.squares]
		for _, _, l, r in self.squares:
			c_min, c_max = sorted((l.real, r.real))
			if c_min < self.left:
				self.left = c_min
			elif c_max > self.right:
				self.right = c_max
			c_min, c_max = sorted((l.imag, r.imag))
			if c_min < self.bottom:
				self.bottom = c_min
			elif c_max > self.top:
				self.top = c_max
		self.tolerance *= 0.8
		#print(self.squares)
	
	def draw(self, screen):
		a_min = (self.top - self.bottom)*(self.right - self.left)
		a_max = (self.top - self.bottom + 2*self.tolerance)*(self.right - self.left + 2*self.tolerance)
		print("Area = {} to {}".format(a_min, a_max))
		for square in self.squares:
			pygame.draw.polygon(screen, 0xFF00FF00, tuple(map(to_screen, square)))
		pygame.draw.polygon(screen, 0xFFFF0000, tuple(map(to_screen, (self.left + self.top*1J, self.right + self.top*1J, self.right + self.bottom*1J, self.left + self.bottom*1J))), width=1)

clock = pygame.time.Clock()
tree = PythTree()

screen.fill(0xFFFFFFFF)
tree.draw(screen)
while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
			pygame.quit()
			sys.exit(0)
		if e.type == pygame.KEYDOWN and e.key == pygame.K_RIGHT:
			tree.update()
			tree.draw(screen)
	#clock.tick(60)
	pygame.display.flip()

