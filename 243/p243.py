from collections import defaultdict
from fractions import Fraction

def factorize(n):
	ret = defaultdict(int)
	for p in [2, 3, 5, 7, 11, 13, 17, 19, 23]:
		if p*p > n:
			break
		while n % p == 0:
			ret[p] += 1
			n //= p
	if n != 1:
		ret[n] += 1
	return ret

def combine(x1f, kf):
	return {p: x1f[p] + kf[p] for p in x1f}

def R(cf):
	n = 1
	d = 1
	for p, k in cf.items():
		if k:
			n *= p**(k - 1)*(p - 1)
			d *= p**k
	return Fraction(n, d - 1)

x1f = {2: 1, 3: 1, 5: 1, 7: 1, 11: 1, 13: 1, 17: 1, 19: 1, 23: 1}
kfs = [factorize(n) for n in range(2, 29)]
L = Fraction(15499, 94744)

for kf in kfs:
	cf = combine(x1f, kf)
	n_d = R(cf)
	if n_d < L:
		print(n_d.denominator + 1)
		break
		

