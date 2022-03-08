from sympy import Symbol, Poly
from sympy.core.numbers import Float
from collections import defaultdict

a = Symbol("a", domain="R")

def zero_poly():
	return Poly(0, a, domain="Z")

def next_pdf(last_pdf, x):
	res = defaultdict(zero_poly)
	for n, p in last_pdf.items():
		res[n] += x*a*p
		if n + 1 <= 20:
			res[n + 1] += (1 - x*a)*p
	return res

pdf = {0: Poly(1, a, domain="Z")}

for x in range(1, 51):
	pdf = next_pdf(pdf, x)

print("Finding roots of pdf[20]=1/50...")

q = 1/next(a for a in (50*pdf[20] - 1).nroots(n=12, maxsteps=500) if isinstance(a, Float) and 0 < a < 1/50)
print(q)

