from collections import Counter

cell_options = [
	[
		[],#last_vert == -1, bottom == -1
		[(-1,0),(0,-1)],#last_vert == -1, bottom == 0
		[(0,0)]#last_vert == -1, bottom == 1
	],
	[
		[(-1,0),(0,-1)],#last_vert == 0, bottom == -1
		[(-1,1),(1,-1)],#last_vert == 0, bottom == 0
		[(1,0),(0,1)]#last_vert == 0, bottom == 1
	],
	[
		[(0,0)],#last_vert == 1, bottom == -1
		[(1,0),(0,1)],#last_vert == 1, bottom == 0
		[]#last_vert == 1, bottom == 1
	]
]

def fill_row(prev_row, cur_row, last_vert, count, ways):
	#print("Filling row ", cur_row)
	if len(prev_row) == len(cur_row):
		if last_vert == 0:
			#print("{} -> {}".format(prev_row, cur_row))
			ways[cur_row] += count
		return
	bottom = prev_row[len(cur_row)]
	for here_vert, top in cell_options[last_vert + 1][bottom + 1]:
		fill_row(prev_row, cur_row + (top,), here_vert, count, ways)

def next_ways(ways):
	n_ways = Counter()
	for prev_row, count in ways.items():
		fill_row(prev_row, (), 0, count, n_ways)
	return n_ways

N = 10

ways = Counter()
fill_row((0,)*N, (-1,), 1, 1, ways)
for t in range(N//2-1):
	ways = next_ways(ways)

total = 0
for half, count in ways.items():
	total += count*(ways[half[::-1]] + ways[tuple(-d for d in half[::-1])])
total *= 2
print(total)

