from functools import cache

@cache
def f(d, n, a, b):
	if n == 1:
		return 1 if a == b == d else 0
	if d == a:
		return f(a + 1, n - 1, a, b) + f(a + 1, n - 1, a + 1, b)
	elif d == b:
		return f(b - 1, n - 1, a, b) + f(b - 1, n - 1, a, b - 1)
	if d <= a or b <= d:
		return 0
	return (f(d - 1, n - 1, a, b) + f(d + 1, n - 1, a, b))

def ways(LEN):
	res = 0
	for d in range(1, 10):
		for n in range(10, LEN + 1):
			res += f(d, n, 0, 9)
	return res

print(ways(40))

