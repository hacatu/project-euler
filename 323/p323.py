#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from fractions import Fraction as Q
from functools import reduce

def C(n, r):
	r = min(r, n - r)
	if r < 0:
		return 0
	return reduce(lambda c, i: c*(n-i)//(i+1), range(r), 1)

#EN = Q(1, 4294967296)
#for j in range(32):
#	x = Q(1, 2**(32-j))
#	EN += C(32, j)*(2-x)*x*(1-x)**-2

def P(n):
	ret = 0
	for j in range(32):
		p_i_1_j_set = C(32, j)*(2**(n - 2) - 1)**j/2**(32*(n - 2))
		p_i_32_set = 1/2**(32 - j)
		ret += p_i_1_j_set*p_i_32_set
	return ret

print(sum(i*P(i) for i in range(1, 53)))
print(float(-sum(C(32, i)*(-1)**i/(1-Q(1,2**i)) for i in range(1, 33))))

#print(float(EN))

