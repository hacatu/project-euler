from sympy import symbols, integrate, Integral, diff, solveset, Eq, pi, sqrt, N

t, v = symbols("t v", positive=True)

x = (1-t)**3 + 3*(1-t)**2*t + 3*(1-t)*t**2*v
y = 3*(1-t)**2*t*v + 3*(1-t)*t**2+t**3

x_ = diff(x, t)
y_ = diff(y, t)

A = integrate(y*x_, (t, x.subs(t, 0), x.subs(t, 1)))

v0 = next(z for z in solveset(Eq(A, pi/4), v) if z < 2)

L = Integral(sqrt(x_**2 + y_**2).subs(v, v0), (t, 0, 1))

print(N(100*(L - pi/2)/(pi/2), 14))

