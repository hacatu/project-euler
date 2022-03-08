from sympy.ntheory import factorint

M = 10**18
MAX = 10**7

res = 0
for k in range(1, MAX + 1):
	res = (res + max(factorint(4*k*k + 1).keys())) % M
	if(k % 10000 == 0):
		print(k)
print(res)

