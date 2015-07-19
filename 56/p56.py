def digitSum(n):
	s = 0
	while n:
		s += n%10
		n //= 10
	return s

max = 1

for i in range(1,101):
	for j in range(1,101):
		sum = digitSum(i**j)
		if sum > max:
			max = sum

print(max)
