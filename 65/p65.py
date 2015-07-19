#print("Enter the covergant of e to calculate:")
#MAX = int(input())
MAX = 100

def gcd(a, b):
	if b > a:
		b, a = a, b
	while True:
		a %= b
		if a == 0:
			return b
		b %= a
		if b == 0:
			return a

def e(i):
	if i%3 == 2:
		return 2*(i + 1)//3
	if i != 0:
		return 1
	return 2

def addToRec(i, n, d):
	d, n = n, d
	n += i*d
	g = gcd(n, d)
	if g != 1:
		n //= g
		d //= g
	return (n, d)

def digitSum(n):
	s = 0
	while n:
		s += n%10
		n //= 10
	return s

n = e(MAX - 1)
d = 1
#print(n, d)
for i in range(0, MAX - 1)[::-1]:
	(n, d) = addToRec(e(i), n, d)
	#print(n, d)

#print(n, d)
print(digitSum(n))
