"""
A 30x30 grid contains a flea on each square.  Each iteration they move to an adjacent square
at random.  After 50 iterations, what is the expected number of unoccupied squares?

Notice two facts: initially, the expected number of fleas per square is 1 because of the initial
condition.  Also, every flea moves independantly, so the expected number of fleas in a square
at iteration k + 1 is the sum of the expected number in each of the adjacent squares at iteration k
times the probability of a flea in that square moving to the square.

If we index A[i][j][k], A[i][j][k + 1] depends on A[i][j - 1][k] and so on, but for example
A[0][1][k + 1] = A[0][0][k]/2 + A[0][2][k]/3 + A[1][1][k]/4, so we see the denomintor goes up by
12x.  Obviously the total expected value is 30*30*1 900 so no square can have more than 900.
900*12^50 < 2**190 so 190 bit numbers are sufficient.
Start with each cell set to 12**50 and divide by 12**50 at the end.
"""

import random
import itertools
import collections

def moveFleas(board, c, ns):
	for i, j in random.choices(ns, k=c):
		board[i][j] += 1

def randBoard():
	board = [[1]*30 for i in range(30)]
	for k in range(50):
		_board = [[0]*30 for i in range(30)]
		moveFleas(_board, board[0][0], [(0, 1), (1, 0)])
		moveFleas(_board, board[0][29], [(0, 28), (1, 29)])
		moveFleas(_board, board[29][0], [(29, 1), (28, 0)])
		moveFleas(_board, board[29][29], [(29, 28), (29, 28)])
		for j in range(1, 29):
			moveFleas(_board, board[0][j], [(0, j - 1), (1, j), (0, j + 1)])
			moveFleas(_board, board[29][j], [(28, j), (29, j - 1), (29, j + 1)])
		for i in range(1, 29):
			moveFleas(_board, board[i][0], [(i - 1, 0), (i, 1), (i + 1, 0)])
			moveFleas(_board, board[i][29], [(i - 1, 29), (i, 28), (i + 1, 29)])
			for j in range(1, 29):
				moveFleas(_board, board[i][j], [(i - 1, j), (i, j - 1), (i, j + 1), (i + 1, j)])
		board = _board
	return board

#Counter({1: 3314405, 0: 3308263, 2: 1655080, 3: 551994, 4: 137721, 5: 27438, 6: 4411, 7: 600, 8: 79, 9: 9})

c = collections.Counter()
for k in range(10000):
	c.update(itertools.chain(*randBoard()))
	print(k, c)

