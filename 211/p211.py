from math import sqrt

def sigma_x_sieve(m, x):
	buf = [1]*m
	for p in range(2, m):
		if buf[p] != 1:
			continue
		px = p**x
		buf[p] = px + 1
		for n in range(2*p, m, p):
			pxa = px
			d = n
			while d%p == 0:
				pxa *= px
				d /= p
			buf[n] *= (pxa - 1)//(px - 1)
	return buf

def is_square(n):
	r = int(sqrt(n))
	return r**2 == n

#M = 64000000
M = 640000

s = sum(n for (n, s) in enumerate(sigma_x_sieve(M, 2)[1:], 1) if is_square(s))
print(s)

