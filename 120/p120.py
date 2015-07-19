print(sum(max(2*n*a%(a*a) for n in range(1, a*a, 2)) for a in range(3, 1001)))
print(sum(max(((a - 1)**n + (a + 1)**n)%(a*a) for n in range(0, a)) for a in range(3, 1001)))
