from fractions import Fraction as Q

MAX = 18

capss = [set(), {Q(1,1)}]

for n in range(2, MAX + 1):
	caps = set()
	for a in range(1, n//2 + 1):
		for c1 in capss[a]:
			for c2 in capss[n - a]:
				caps.add(c1 + c2)
				caps.add(1/(1/c1 + 1/c2))
	capss.append(caps)
print(len(set.union(*capss)))

