from fractions import Fraction
from collections import defaultdict

MIN = 2
MAX = 45

sums = defaultdict(int)

iss = [Fraction(1, i**2) for i in range(MIN, MAX + 1)]

sums[Fraction()] += 1;

target = sum(iss) - Fraction(1, 2)

for i in iss:
	print(i)
	nsums = defaultdict(int)
	for s in sums:
		if s + i > target:
			continue
		nsums[s + i] += 1
	for s in nsums:
		sums[s] += nsums[s]

print(sums[Fraction(1, 2)])

