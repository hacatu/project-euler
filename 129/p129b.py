def A(n):
	k = 1
	while pow(10, k, n) != 1:
		k += 1
	return k

print(A(41))

n = 16
while True:
	n += 1
	if n%2 == 0 or n%5 == 0:
		continue
	a = A(n)
	print(n, a)
	if a > 1000:
		break

