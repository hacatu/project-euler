"""
Heads multiplies our money by (1+2f) and tails multiplies by (1-f),
so to maximize the probability of breaking a billion we need to
find the minimal k so that (1+2f)**k*(1-f)**(1000-k) >= 10**9
has a solution for f in (0,1).  For any number of heads at least
k, getting that many heads with that f puts us over a billion.
Taking the logarithm and graphing, we see that k = 432 passes over 0
but k = 431 is always negative so k = 432.

It suffices to check f = (3k-1000)/2000, the maximizer.

With this k in hand, we just need to find P(B(1000,1/2)>=432)
=1/2 + (P(432) + ... + P(499))
"""
from math import comb as binom, sqrt
from statistics import NormalDist as N

print("Exact Probability:    {}".format(sum(binom(1000, k) for k in range(432, 1000))/2**1000))
print("Normal Approximation: {}".format(N(500, sqrt(250)).cdf(1000 - 432 + 0.5)))

