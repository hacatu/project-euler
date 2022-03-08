from functools import reduce

N = 10**15
#N = 7
X = 100
fib15 = 1307674368000
M = fib15*5*11**3

mpoly = (0, 1)
mpsq = (1, 0)
n = N
while n:
	if n&1:
		mpoly = ((mpsq[0]*(mpoly[0] + mpoly[1]) + mpsq[1]*mpoly[0])%M, (mpsq[0]*mpoly[0] + mpsq[1]*mpoly[1])%M)
	mpsq = (mpsq[0]*(mpsq[0] + (mpsq[1]<<1))%M, (mpsq[0]*mpsq[0] + mpsq[1]*mpsq[1])%M)
	n >>= 1

fibN1 = (mpoly[0] + mpoly[1])%M
fibN = mpoly[0]

def pow_in(n, p):
	if n == 0:
		return 0#Uh Oh
	a = 1
	while n%p == 0:
		a *= p
		n //= p
	return a

moduli = {p:pow_in(fib15, p) for p in [2, 3, 5, 7, 11, 13]}
residues = {}

#print(moduli)

def bezout(a, b):
	r0 , r1 = b, a
	s0, s1 = 1,  0
	t0, t1 = 0, 1
	while(r1):
		q = r0//r1
		r0, r1 = r1, r0%r1
		s0, s1 = s1, s0 - q*s1
		t0, t1 = t1, t0 - q*t1
	return (t0, s0)

def mmi(a, b):
	ai = bezout(a, b)[0]
	if ai < 0:
		ai += b
	return ai

for p, pa in moduli.items():
	s = 0
	for x in range(X + 1):
		if (x**2 + x - 1)%p:
			s = (s + ((fibN1 + fibN*x)*pow(x, N + 1, pa) - x)*mmi(x**2 + x - 1, pa))%pa
		else:
			pb = pow_in(x**2 + x - 1, p)
			n = ((fibN1 + fibN*x)*pow(x, N + 1, pb*pa) - x)%(pb*pa)
			d = (x**2 + x - 1)//pb
			n //= pb
			s = (s + n*mmi(d, pa))%pa
	residues[pa] = s

#print(residues)

def crt(r1, n1, r2, n2):
	t0, s0 = bezout(n1, n2)
	return (r2*t0*n1 + r1*s0*n2)%(n1*n2)

it = iter(residues.items())
n0, r0 = next(it)
for ni, ri in it:
	n0, r0 = n0*ni, crt(r0, n0, ri, ni)

for ni, ri in residues.items():
	if r0%ni != ri:
		print("Oopsie")

print("{} mod {}".format(r0, n0))

