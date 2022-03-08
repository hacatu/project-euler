from math import sqrt
from itertools import permutations
from operator import itemgetter

def plen(rs):
	R = max(rs)
	return rs[0] + rs[-1] + sqrt(R)*sum(sqrt(2*(rs[i] + rs[i+1]) - R) for i in range(len(rs) - 1))

rs, x = min(((rs, plen(rs)) for rs in permutations(range(43, 51)) if rs[0] > rs[-1]), key=itemgetter(1))
print("{}: {}".format(rs, x))

