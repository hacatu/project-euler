from math import sqrt
from operator import itemgetter

radii = list(range(30, 51))

bound = 1 << (len(radii) - 2)

R = max(radii)

def len_for_mask(mask):
	a, b = radii[:2]
	x = sqrt(a + b - R)
	for r in radii[2:]:
		if mask & 1:
			x += sqrt(b + r - R)
			b = r
		else:
			x += sqrt(r + a - R)
			a = r
		mask >>= 1
	return a + b + 2*sqrt(R)*x

def permutation_for_mask(mask):
	rs = radii[:2]
	for r in radii[2:]:
		if mask & 1:
			rs.append(r)
		else:
			rs = [r] + rs
		mask >>= 1
	return rs


mask, mlen = min(((mask, len_for_mask(mask)) for mask in range(bound)), key=itemgetter(1))
print("{}: {}".format(permutation_for_mask(mask), mlen))

#print("{}: {}".format(permutation_for_mask(0b101010101010101010), len_for_mask(0b101010101010101010)))

