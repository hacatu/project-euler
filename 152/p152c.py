from heapq import heappush, heapreplace, merge
from itertools import takewhile, accumulate, repeat
from collections import OrderedDict, defaultdict
from functools import reduce
from operator import mul

MAX = 80
TARGET = 2

def maxpow_lt(n, m):
	r = 1
	while r*n <= m:
		r *= n
	return r

def maxpow_in(n, m):
	r = 1
	while m%(r*n) == 0:
		r *= n
	return r

def primes():
	g = [(4, 2)]
	n = 2
	while True:
		if n < g[0][0]:
			heappush(g, (n*n, n))
			yield n
			n += 1
		elif n > g[0][0]:
			heapreplace(g, (g[0][0] + g[0][1], g[0][1]))
		else:
			n += 1
			heapreplace(g, (g[0][0] + g[0][1], g[0][1]))

basis = OrderedDict((p, None) for p in takewhile(lambda p: p <= MAX, primes()))

while True:
	for p in reversed(basis):
		if TARGET%p == 0:
			continue
		if reduce(mul, (maxpow_lt(q, MAX//p)**2 for q in basis)) < p**2:
			del basis[p]
			break
	else:
		break

basis = [p for p in basis]

orths = [[1]]
for p in basis:
	orths.append(list(merge(*(takewhile(lambda n: n <= MAX//p, accumulate(repeat(n), lambda n, _: n*p)) for n in orths[-1]))))

orths = orths[1:]

denom = reduce(mul, (maxpow_lt(p, MAX)**2 for p in basis))

target_basis = [maxpow_in(p, denom)//maxpow_in(p, TARGET) for p in basis]

def gen_groups(base, i):
	k = 0
	msums = [0]*(len(basis) + 1)
	gcode = 0
	mod = target_basis[i]
	s = defaultdict(list)
	while len(base):
		s[msums[i]%mod].append(msums[:])
		k += 1
		xbit = k&-k
		gcode ^= xbit
		if 1 << len(base) <= gcode:
			return s
		xi = xbit.bit_length() - 1
		delta = 1 if gcode&xbit else -1
		delta *= denom//(base[xi]*basis[i])**2
		msums[-1] += delta
		msums[i] += delta
		for j in range(i):
			if base[xi]%basis[j] == 0:
				msums[j] += delta
	return s

agroups = [None]*len(basis)
bgroups = [None]*len(basis)
for i, (orth, p) in enumerate(zip(orths, basis)):
	aset = orth[:len(orth)//2]
	bset = orth[len(orth)//2:]
	agroups[i] = gen_groups(aset, i)
	bgroups[i] = gen_groups(bset, i)

def match_groups_rec(i, msums):
	if i == -1:
		return 1 if msums[-1] == denom//TARGET else 0
	ret = 0
	mod = target_basis[i]
	for amod, agroup in agroups[i].items():
		#amod + bmod + msums[i] == 0 (mod target_basis[i])
		bmod = (-amod - msums[i]) % mod
		for bset in bgroups[i][bmod]:
			bsums = [m + b for m, b in zip(msums, bset)]
			for aset in agroup:
				ret += match_groups_rec(i - 1, [b + a for b, a in zip(bsums, aset)])
	return ret

print(match_groups_rec(len(basis) - 1, [0]*(len(basis) + 1)))

