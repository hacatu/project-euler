from math import log
n = 1e12

e2 = int(log(n, 2))
e3 = int(log(n, 3))
e5 = int(log(n, 5))

result = [1]
for i in range(1, e2 + 1):
	a = 2**i
	for j in range(e3 + 1):
		b = 3**j
		if a * b > n:
			break
		for k in range(e5 + 1):
			c = 5**k
			x = a * b * c
			if x > n:
				break
			result.append(x)
print(len(result))

