import numpy as np
import itertools as itt
import pygame
import cmath
import sys
import time
from collections import defaultdict

GT_EPSILON = 1e-8
WIDTH = 800

def fst(t):
	return t[0]

def snd(t):
	return t[1]

def toScreen(z):
	return (int(z.real*WIDTH + WIDTH/2), int(WIDTH/2 - z.imag*WIDTH))

def fromScreen(p):
	return p[0]/WIDTH - 0.5 - (p[1]/WIDTH - 0.5)*1J

def complexCross(a, b):
	return a.real*b.imag - a.imag*b.real

def intersectLines(a_p, a_o, b_p, b_o):
	det = complexCross(a_o, b_o);
	if abs(det) < GT_EPSILON:
		if abs(complexCross(b_p - a_p, a_o)) < GT_EPSILON:#collinear
			return a_p
		return None
	t = complexCross(b_p - a_p, a_o)/det
	return b_p + b_o*t

def triangleContains(triangle, point):
	for i in range(3):
		offset = point - triangle[i]
		side = triangle[(i + 1)%3] - triangle[i]
		if complexCross(side, offset) <= -GT_EPSILON:
			return False
	return True

def drawLine(screen, color, triangle, direction, point):
	inters = [
		intersectLines(triangle[0], triangle[1] - triangle[0], point, direction),
		intersectLines(triangle[1], triangle[2] - triangle[1], point, direction),
		intersectLines(triangle[2], triangle[0] - triangle[2], point, direction)
	]
	"""
	for p in inters:
		if p is not None:
			if not triangleContains(triangle, p):
				pygame.draw.circle(screen, 0xFFFF0000, toScreen(p), 5)
			else:
				pygame.draw.circle(screen, 0xFF00FF00, toScreen(p), 5)
	"""
	for i, p in enumerate(inters):
		if p is None:
			#print("Line is parallel to one side")
			if not triangleContains(triangle, inters[(i+1)%3])or abs(inters[(i+1)%3]-inters[(i-1)%3]) <= GT_EPSILON:
				return False
			pygame.draw.line(screen, color, toScreen(inters[(i-1)%3]), toScreen(inters[(i+1)%3]))
			return True
	for i, p in enumerate(inters):
		if not triangleContains(triangle, p):
			if not triangleContains(triangle, inters[(i+1)%3]) or abs(inters[(i+1)%3]-inters[(i-1)%3]) <= GT_EPSILON:
				return False
			pygame.draw.line(screen, color, toScreen(inters[(i-1)%3]), toScreen(inters[(i+1)%3]))
			return True
	#print("Line intersects a vertex or is colinear to a side")
	corners = []
	off_corners = []
	for p in inters:
		if min(abs(v - p) for v in triangle) <= GT_EPSILON:
			corners.append(p)
		else:
			off_corners.append(p)
	if off_corners:
		pygame.draw.line(screen, color, toScreen(off_corners[0]), toScreen(corners[0]))
	else:
		i = max(range(3), key=lambda i:min(abs(triangle[i] - p) for p in corners))
		pygame.draw.line(screen, color, toScreen(inters[(i-1)%3]), toScreen(inters[(i+1)%3]))
	return True

def drawParallelLines(screen, color, triangle, direction, point, offset):
	p = point
	while drawLine(screen, color, triangle, direction, p):
		p += offset
	p = point - offset
	while drawLine(screen, color, triangle, direction, p):
		p -= offset

pygame.init()
screen = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("Triangles, Triangles")
T = [0.5*cmath.exp(2*cmath.pi*(i/3 + 1/4)*1J) for i in range(3)]

N = 8

toggles = defaultdict(lambda:True)

while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
			pygame.quit()
			sys.exit(0)
		if e.type == pygame.MOUSEBUTTONDOWN:
			p = pygame.mouse.get_pos()
			if triangleContains(T, fromScreen(p)):
				print("Clicked in the triangle at", fromScreen(p))
			else:
				print("Clicked outside the triangle at", fromScreen(p))
		elif e.type == pygame.KEYDOWN:
			if e.key < 256 and chr(e.key) in toggles.keys():
				toggles[e.unicode] ^= True
			if e.key == pygame.K_LEFT and N > 1:
				N -= 1
			elif e.key == pygame.K_RIGHT:
				N += 1
	screen.fill(0xFF000000)
	if toggles["a"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#horizontal lines
			T[2] - T[1], T[0]*(1/N) + T[1]*((N-1)/N), (T[1] - T[0])*(1/N))
	if toggles["s"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#positive orth lines
			T[1] - T[0], T[2]*(1/N) + T[0]*((N-1)/N), (T[0] - T[2])*(1/N))
	if toggles["d"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#negative orth lines
			T[0] - T[2], T[1]*(1/N) + T[2]*((N-1)/N), (T[2] - T[1])*(1/N))
	if toggles["q"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#vertical diag lines
			T[0], 0J, 0.25*cmath.sqrt(3)*(1/N))
	if toggles["e"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#positive diag lines
			T[1], 0J, 0.25*cmath.sqrt(3)*cmath.exp(2*cmath.pi*1/3*1J)*(1/N))
	if toggles["w"]:
		drawParallelLines(screen, 0xFFFF00FF, T,#negative diag lines
			T[2], 0J, 0.25*cmath.sqrt(3)*cmath.exp(-2*cmath.pi*1/3*1J)*(1/N))
	for i in range(3):
		pygame.draw.line(screen, 0xFFFFFFFF, toScreen(T[i]), toScreen(T[(i+1)%3]))
	pygame.display.flip()
	#time.sleep(100)

