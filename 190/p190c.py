"""
x_1 + ... + x_m = m
x_m = m - x_1 - ... - x_m
P_m = x_1^1 * ... * x_m^m = max
L_m = 1*ln(x_1) + ... + m*ln(x_m) = max
L_m = 1*ln(x_1) + ... + (m-1)*ln(x_(m-1)) + m*ln(m - x_1 - ... - x_(m-1))

dL_m/dx_i = 1/x_i - m/(m - x_1 - ... - x_(m-1)) = 0

i/x_i = m/x_m

x_1/1 = ... = x_m/m = l

x_i = il

x_1 + ... + x_m = m(m+1)/2*l = m

l = 2/(m + 1)


x_i = 2i/(m + 1)

P_m = (2/(m + 1))^(m*(m+1)/2) * 1^1 * ... * m^m

"""

def P(m):
	e = (m*(m + 1)) >> 1
	p = 1
	for i in range(1, m + 1):
		p *= i**i
	if m & 1:
		q = ((m + 1) >> 1)**e
	else:
		p <<= e
		q = (m + 1)**e
	return p // q

print(sum(P(m) for m in range(2, 16)))

