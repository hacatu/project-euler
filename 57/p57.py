from fractions import Fraction
from math import log10, floor
sr2 = Fraction(1,1)
one = Fraction(1,1)
c = 0;
for i in range(1,801):
	sr2 = one + one/(one + sr2)
	if floor(log10(sr2.numerator))>floor(log10(sr2.denominator)):
		c += 1
print(c)
