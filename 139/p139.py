MAX = 100000000

"""
Pythagorean triples (a, b, c) | (b - a)|c | a + b + c < 100000000
Generating pythagorean triples:
a = k*(m^2 - n^2), b = k*(2*m*n), c = k*(m^2 + n^2), perim = 2*k*(m^2 + m*n)
m > n.  m and n are coprime.  Start with m - n is odd.
"""

def gcd(a, b):
	while True:
		if a == 0:
			return b
		b %= a
		if b == 0:
			return a
		a %= b

c = 0;
m = 1
while 2*(m*m + m) < MAX:
	n = 2 if m%2 == 1 else 1
	while n < m:
		if 2*(m*m + m*n) >= MAX:
			break
		if gcd(m, n) != 1:
			n += 2
			continue
		a0 = m*m - n*n;
		b0 = 2*m*n;
		c0 = m*m + n*n;
		if c0%(b0 - a0):
			n += 2
			continue
		c += MAX//(a0 + b0 + c0);
		n += 2
	m += 1
print(c)

