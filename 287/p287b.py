import pygame

WIDTH = 800

pygame.init()
screen = pygame.display.set_mode((WIDTH, WIDTH))
pygame.display.set_caption("Project Euler 287")

N = 5

tN = 1 << N

while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
			pygame.quit()
			sys.exit(0)
	screen.fill(0xFFFFFFFF)
	for x in range(1 << N):
		for y in range(x + 1):
			if (x - (1 << (N - 1)))**2 + (y - (1 << (N - 1)))**2 <= 1 << (2*N - 2):
				pygame.draw.rect(screen, 0xFF000000, (x*WIDTH/tN, y*WIDTH/tN, WIDTH/tN, WIDTH/tN))
				pygame.draw.rect(screen, 0xFF000000, (y*WIDTH/tN, x*WIDTH/tN, WIDTH/tN, WIDTH/tN))
	pygame.display.flip()

