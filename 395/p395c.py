import pygame
import sys
import cmath
import numpy as np
import itertools as itt

SCREEN_WIDTH = 1200

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_WIDTH))
pygame.display.set_caption("Broccoli")

def to_screen(z):
	z = SCREEN_WIDTH/9*(z + 4 + 1J)
	return (z.real, z.imag)

class PythTree:
	T_L = (16/25 + 12J/25, 1J)
	T_R = (9/25 - 12J/25, 16/25 + 37J/25)
	root = 16/25 + 37J/25
	
	def __init__(self):
		self.ts = [(PythTree.root, 1, 0)]
		self.bottom = 0
		self.top = 1
		self.left = 0
		self.right = 1
		self.tolerance = 5*np.sqrt(2)

	def update(self):
		self.tolerance *= 0.8
		self.ts = [(T[0]*a*PythTree.root + T[1]*a + c, T[0]*a, T[1]*a + c) for T in (PythTree.T_L, PythTree.T_R) for (z, a, c) in self.ts]
		for z, _, _ in self.ts:
			if z.real < self.left:
				self.left = z.real
			elif z.real > self.right:
				self.right = z.real
			if z.imag < self.bottom:
				self.bottom = z.imag
			elif z.imag > self.top:
				self.top = z.imag
		#print(self.squares)
	
	def prune(self):
		self.ts = [(z, a, c) for (z, a, c) in self.ts if (z.real - self.left) < self.tolerance or (self.right - z.real) < self.tolerance or (z.imag - self.bottom) < self.tolerance or (self.top - z.imag) < self.tolerance]
	
	def draw(self, screen):
		a_min = (self.top - self.bottom)*(self.right - self.left)
		a_max = (self.top - self.bottom + 2*self.tolerance)*(self.right - self.left + 2*self.tolerance)
		pygame.draw.polygon(screen, 0xFFFF0000, tuple(to_screen(x + y*1j) for (x, y) in zip((self.left, self.right, self.right, self.left), (self.top, self.top, self.bottom, self.bottom))), width=1)
		pygame.draw.polygon(screen, 0xFFFF0000, tuple(to_screen(x + y*1j) for (x, y) in zip((self.left + self.tolerance, self.right - self.tolerance, self.right - self.tolerance, self.left + self.tolerance), (self.top - self.tolerance, self.top - self.tolerance, self.bottom + self.tolerance, self.bottom + self.tolerance))), width=1)
		print("Points: {}".format(len(self.ts)))
		print("Area = {} to {}".format(a_min, a_max))
		for z, _, _ in self.ts:
			pygame.draw.circle(screen, 0xFF00FF00, to_screen(z), max(SCREEN_WIDTH/180*self.tolerance, 1))

clock = pygame.time.Clock()
tree = PythTree()

screen.fill(0xFFFFFFFF)
tree.draw(screen)
while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
			pygame.quit()
			sys.exit(0)
		elif e.type == pygame.KEYDOWN:
			if e.key == pygame.K_RIGHT:
				tree.update()
				screen.fill(0xFFFFFFFF)
				tree.draw(screen)
			elif e.key == pygame.K_UP:
				tree.prune()
				screen.fill(0xFFFFFFFF)
				tree.draw(screen)
	#clock.tick(60)
	pygame.display.flip()

