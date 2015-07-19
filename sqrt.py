def isqrt(n):
    x = n
    y = (x + 1) // 2
    while y < x:
        x = y
        y = (x + n // x) // 2
    return x

def digitSum(n):
	sum = 0
	for i in range(0, 100):
		sum += n%10
		n //= 10
	return sum
