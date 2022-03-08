#!/usr/bin/env python3

def farey_seq(n):
    a, b, c, d = 0, 1, 1, n
    yield a, b
    while c <= n:
        k = (n + b) // d
        a, b, c, d = c, d, k*c - a, k*d - b
        yield a, b

def bcnd_count_seq(n):
    yield from (len([0 for a, b in farey_seq(k) if a + b <= k + 1]) for k in range(n))

for i, c in enumerate(bcnd_count_seq(14)):
    print("{}: {}".format(i, c))
