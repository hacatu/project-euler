from fractions import Fraction

def crossP(ax, ay, bx, by):
	return ax*by - ay*bx

def intersection(px, py, rx, ry, qx, qy, sx, sy):
	a = crossP(rx, ry, sx, sy)
	if a == 0:
		return None
	dx = Fraction(qx - px, a)
	dy = Fraction(qy - py, a)
	t = crossP(dx, dy, rx, ry)
	if t <= 0 or t >= 1:
		return None
	t = crossP(dx, dy, sx, sy)
	if t <= 0 or t >= 1:
		return None
	return (px + t*rx, py + t*ry)

t = [None]*20000
xs = set()
s = 290797*290797%50515093
for n in range(20000):
	t[n] = s%500
	s = s*s%50515093
for i in range(0, 20000, 4):
	t[i + 2] -= t[i]
	t[i + 3] -= t[i + 1]
for i in range(0, 19996, 4):
	for j in range(i + 4, 20000, 4):
		p = intersection(t[i], t[i + 1], t[i + 2], t[i + 3], t[j], t[j + 1], t[j + 2], t[j + 3])
		if p is not None:
			xs.add(p)
print(len(xs))

