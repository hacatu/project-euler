def cand(*args):
	wheel = 1
	for p in args:
		wheel *= p
	ret = []
	for k in range(wheel):
		y = (4*k*k + 1)%wheel
		if any(y%p == 0 for p in args):
			ret.append(k)
	return ret

