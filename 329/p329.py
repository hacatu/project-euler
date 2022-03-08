from fractions import Fraction as Q
from math import sqrt, floor

def sieve_primes(m):
	is_prime = [False]*2 + [True]*(m - 1)
	for p in range(2, floor(sqrt(m)) + 1):
		if not is_prime[p]:
			continue
		for k in range(p*p, m + 1, p):
			is_prime[k] = False
	return is_prime

is_prime = sieve_primes(500)

_1_2 = Q(1, 2)
_1_3 = Q(1, 3)
_2_3 = Q(2, 3)

def croak(k, p):
	return _2_3 if is_prime[k] == ('P' == p) else _1_3

def P(seq):
	table = [0] + [Q(1, 500)*croak(k, seq[0]) for k in range(1, 501)]
	seq = seq[1:]
	while len(seq) > 0:
		table = ([0, _1_2*table[2]*croak(1, seq[0]), (table[1] + _1_2*table[3])*croak(2, seq[0])] +
		         [_1_2*(table[k-1] + table[k+1])*croak(k, seq[0]) for k in range(3, 499)] +
		         [(_1_2*table[498] + table[500])*croak(499, seq[0]), _1_2*table[499]*croak(500, seq[0])])
		seq = seq[1:]
	return sum(table[1:])

print(P("PPPPNNPPPNPPNPN"))

