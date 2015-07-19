a = b = 1
i = 2
while a < 10**999:
    a, b = a + b, a
    i += 1

print(i)
