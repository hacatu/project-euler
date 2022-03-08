def fpow(n, f):
	r = 1
	while n%f == 0:
		n //= f
		r *= f
	return r

MAX = 10**12
MOD = 10**5
P = 7

f = 1*2*3*4
t = MAX//5

for i in range(5, MOD, 5):
	f *= i*(i + 1)*(i + 2)*(i + 3)*(i + 4)//4//fpow(i, 5)
	f %= MOD

print(f)
f = pow(f, P, MOD)

p = 25
while p <= MAX:
	t -= MAX//p
	p *= 5

print(t)
f *= pow(2, t, MOD)
f %= MOD
print(f)

