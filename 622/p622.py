#from sympy import Matrix
from itertools import product
from functools import reduce


def makeRiffleMatrix(n):
	def o_pred(i, o):
		if i < n//2 - 1:
			return 1 if o == 2*i + 1 else 0
		return 1 if o == 2*(i - n//2 + 1) else 0
	return Matrix(n - 2, n - 2, o_pred)

def order(A):
	P = Matrix(A)
	n = 1
	while not P.is_Identity:
		P *= A
		n += 1
	return n


"""
So we found that order(makeRiffleMatrix(2*n + 2)) is the multiplicative
order of 2 mod 2*n + 1.  2**o == 1 mod 2*n + 1.
We're interested in finding n so that 2**60 == 1 mod 2*n + 1
but 2**30, 2**20, and 2**12 are not 1.
This means 2*n + 1 should divide 2**60 - 1.
2**60 - 1 = 3**2 * 5**2 * 7 * 11 * 13 * 31 * 41 * 61 * 151 * 331 * 1321
2**30 - 1 = 3**2        * 7 * 11      * 31           * 151 * 331
2**20 - 1 = 3    * 5**2     * 11      * 31 * 41
2**12 - 1 = 3**2 * 5    * 7      * 13
So we have

p0 <= 2 && p1 <= 2 && p2 <= 1 && p3 <= 1 && p4 <= 1 && p5 <= 1 && p6 <= 1 && p7 <= 1 && p8 <= 1 && p9 <= 1 && p10 <= 1
&& (p1 >= 1 || p4 >= 1 || p6 >= 1 || p7 >= 1 || p10 >= 1)
&& (p0 >= 2 || p2 >= 1 || p4 >= 1 || p7 >= 1 || p8 >= 1 || p9 >= 1 || p10 >= 1)
&& (p1 >= 2 || p3 >= 1 || p5 >= 1 || p6 >= 1 || p7 >= 1 || p8 >= 1 || p9 >= 1 || p10 >= 1)

How can we most efficiently iterate through solutions to this?
There are 3**2 * 2**9 = 4608 divisors of 2**60 - 1 so iterating over all of those and seeing
which ones don't also divide 2**30 - 1 etc would probably be best.

Currently we're getting the sum of s(n) = 8 is 438 but it should be 412.
We get 18, 6, 86, 4, 52, 16, and 256 so some probably some subset of 4, 6, 16, 18 adds up to 26.
4 + 6 + 16 = 26.  And 2**8 - 1 = 255 which is indeed zero mod 4 - 1, 6 - 1, and 16 - 1
"""

s = 0
factorization = {3: 2, 5: 2, 7: 1, 11: 1, 13: 1, 31: 1, 41: 1, 61: 1, 151: 1, 331: 1, 1321: 1}
max_divisor_mods = [2**d - 1 for d in [12, 20, 30]]
#factorization = {3: 1, 5: 1, 17: 1}
#max_divisor_mods = [2**d - 1 for d in [4]]
for powers in product(*(range(power + 1) for power in factorization.values())):
	d = reduce(lambda prod, prime_power: prod*prime_power[0]**prime_power[1], zip(factorization.keys(), powers), 1)
	if any(m%d == 0 for m in max_divisor_mods):
		continue
	#print(d + 1)
	s += d + 1
print(s)

