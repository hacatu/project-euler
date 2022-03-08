option_map = {
	(i, j): ((i, j), (-j, i), (j, -i)) for (i, j) in ((1,0),(-1,0),(0,1),(0,-1))
}

def foldings_rec(N, pos=(0,0), options=((0,-1),(1,0)), spots=None):
	count = 1
	if N > 1:
		if spots is None:
			spots = set()
		spots.add(pos)
		x, y = pos
		for d in options:
			dx, dy = d
			n_pos = (x + dx, y + dy)
			if n_pos not in spots:
				count += foldings_rec(N-1, n_pos, option_map[d], spots)
		spots.remove(pos)
	return count

