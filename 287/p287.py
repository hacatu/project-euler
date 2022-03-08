N = 24
a = [[0 if (x - (1<<(N-1)))**2 + (y - (1<<(N-1)))**2 <= 1<<(2*N-2) else 1 for x in range(N)] for y in range(N)]

def quadtree(a, l, r, b, t):
	if r - l == 1:
		return "11" if a[l][b] else "10"
	tl = quadtree(a, l, (l + r)//2, (b + t)//2, t)
	tr = quadtree(a, (l + r)//2, r, (b + t)//2, t)
	bl = quadtree(a, l, (l + r)//2, b, (b + t)//2)
	br = quadtree(a, (l + r)//2, r, b, (b + t)//2)
	if tl == tr == bl == br:
		return tl
	return "0" + tl + tr + bl + br

print(quadtree([[1, 1, 0, 0], [1, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]], 0, 4, 0, 4))
print(quadtree())

