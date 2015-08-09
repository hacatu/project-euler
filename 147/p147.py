from sympy import symbols, summation, ceiling, expand, simplify

def even(w, h, n, m):
	return (h + 1 - ceiling(m/2) - ceiling(n/2))*(w + 1 - ceiling((m + n)/2))

def odd(w, h, n, m):
	return (h - ceiling((m - 1)/2) - ceiling((n - 1)/2))*(w - ceiling((m + n)/2))

w, h, n, m = symbols("w h n m", integer="True")

d = summation(summation(even(w, h, n, m) + odd(w, h, n, m), (m, 1, 2*w - n)), (n, 1, 2*w - 1))

print(simplify(expand(d)).subs([(w, 2), (h, 3)]).evalf())

