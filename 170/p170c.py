"""
a*b1 = c1
...
a*bn = cn
a ++ b1 ++ ... ++ bn is pandigital
c1 ++ ... ++ cn is pandigital
maximize c1 ++ ... ++ cn

strategy: decompose descending permutations

for every permutation descending
	for every partition
		find the smallest part (<=87654)
		(precompute all such factorizations)
		for every divisor except 1
			check if it divides the other parts
			check if the result is pandigital
"""

from itertools import permutations

def power_pair(n, p):
	m = 1
	while n%p == 0:
		m *= p
		n //= p
	return (p, m)

factorization = []

for n in range(0, 87655):
	factorization.append([])

p = 2

while p*p <= 87654:
	if len(factorization[p]) == 0:
		for m in range(p, 87654, p):
			factorization[m].append(power_pair(m, p))
	p += 1

def cat(ds, b):
	ret = 0
	for d in ds:
		ret = ret*b + d
	return ret

def divisors(n):
	fact = factorization[n]
	powers = [1]*len(fact)
	divisor = 1
	while True:
		yield divisor
		for i in range(0, len(fact)):
			f = fact[i][0]
			if powers[i] == fact[i][1]:
				divisor //= powers[i]
				powers[i] = 1
				continue
			powers[i] *= f
			divisor *= f
			break
		else:
			break

def digit_mask(n, b, mask):
	while n:
		if mask & (1 << n%b) != 0:
			return 0
		mask &= 1 << n%b
	return mask

def find_partition_rec(tail, parts, min_part):
	if len(tail) > 0:
		for ilen in range(1, len(tail)):
			parts.append(cat(tail[:ilen], 10))
			if find_partition_rec(tail[ilen:], parts, min(min_part, parts[-1])):
				return True
			parts.pop()
		return False
	for divisor in divisors(min_part):
		if divisor == 1:
			continue
		orth = False
		for part in parts:
			if part%divisor != 0:
				orth = True
				break
		if orth:
			continue
		digits = digit_mask(divisor, 10, 0)
		if digits == 0:
			continue
		for part in parts:
			digits = digit_mask(part//divisor, 10, digits)
			if digits == 0:
				orth = True
				break
		if orth:
			continue
		if digits == 2**10 - 1:
			return True
	return False

def find_partition(perm):
	parts = []
	return find_partition_rec(perm, parts, 87654)

for permutation in permutations(range(9, -1, -1)):
	print(cat(permutation, 10))
	if find_partition(permutation):
		break

