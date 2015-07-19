def A(n):
	n *= 9
	k = 1
	while pow(10, k, n) != 1:
		k += 1
	return k

n = 999999
while True:
	n += 2
	if n%5 == 0:
		continue
	a = A(n)
	if a > 1000000:
		break
print(n)

