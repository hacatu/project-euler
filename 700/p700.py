d = 1504170715041707
d_ = 3451657199285664
p = 4503599627370517
t = d
n = 2
s = d
m = d

print("n: 1, sum: {}, min: {}".format(s, m));

while m > 67108864:
	if t < m:
		s += t
		m = t
		print("n: {}, sum: {}, min: {}".format(n, s, t))
	t = (t + d)%p
	n += 1

while True:
	n = min(d_*t % p for t in range(1, m))
	t = d*n % p
	m = t
	s += t
	print("n: {}, sum: {}, min: {}".format(n, s, t))
	if m == 1:
		break

