from pyprimesieve import primes as gen_primes
from bisect import bisect_right as upper_bound
from itertools import takewhile

def powers(p):
	P = p
	while True:
		yield P
		P *= p

def take_upper_bound(m, it):
	return takewhile(lambda e: e <= m, it)

MAX = 100000000000
MIN_C1 = 17**1*13**2*5**3

def sum_ktype_rec(c1primes, c3prods, c3prod_sums, ktype, kprod, kset):
	if not ktype:
		i = upper_bound(c3prods, MAX/kprod)
		return kprod*c3prod_sums[i]
	res = 0
	for p in take_upper_bound((MAX//kprod)**(1/ktype[0]), c1primes):
		if p in kset:
			continue
		res += sum_ktype_rec(c1primes, c3prods, c3prod_sums, ktype[1:], kprod*p**ktype[0], kset | {p})
	return res

def sum_ktype(c1primes, c3prods, c3prod_sums, ktype):
	return sum_ktype_rec(c1primes, c3prods, c3prod_sums, ktype, 1, set())


primes = gen_primes(MAX//(5**3*13**2) + 1)
print("Generated primes")

c1primes = [p for p in primes if p%4 == 1]
c3primes = [p for p in take_upper_bound(MAX//MIN_C1, primes)]

c3prod_flags = [True]*(MAX//MIN_C1 + 1)
for p in take_upper_bound(MAX//MIN_C1, c1primes):
	for m in range(p, MAX//MIN_C1 + 1, p):
		c3prod_flags[m] = False
c3prods = [p for (p, f) in enumerate(c3prod_flags) if f]
print("Generated products of C3 primes")

c3prod_sums = [0]*(len(c3prods) + 1)
for i in range(len(c3prods)):
	c3prod_sums[i + 1] = c3prod_sums[i] + c3prods[i]

S = sum(sum_ktype(c1primes, c3prods, c3prod_sums, ktype) for ktype in ((10, 2), (7, 3), (3, 2, 1)))
print(S)

