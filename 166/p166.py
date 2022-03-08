"""
abcd
efgh
ijkl
mnop
"""
w = 0
for a in range(0, 5):
	for b in range(0, 10):
		for c in range(0, 10):
			for d in range(0, 10):
				s = a + b + c + d
				for e in range(0, 10):
					for f in range(0, 10):
						for g in range(0, 10):
							h = s - (e + f + g)
							if h < 0 or h > 9:
								continue
							for j in range(0, 10):
								n = s - (b + f + j)
								if n < 0 or n > 9:
									continue
								m = s - (j + g + d)
								if m < 0 or m > 9:
									continue
								k = s - (f + g + j)
								if k < 0 or k > 9:
									continue
								i = s - (a + e + m)
								if i < 0 or i > 9:
									continue
								o = s - (c + g + k)
								if o < 0 or o > 9:
									continue
								p = s - (a + f + k)
								if p < 0 or p > 9:
									continue
								l = s - (d + h + p)
								if l < 0 or l > 9:
									continue
								w += 2

print(w)

