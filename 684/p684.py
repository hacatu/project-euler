p = 1000000007

def S(n):
	k = n//9
	m = n%9
	#return (m + 12)*(m + 3)//2*pow(10, k%(p-1), p) - 9*k - m - 2
	#return sum(((i % 9) + 1)*pow(10, i // 9, p) - 1 for i in range(n + 1)) % p
	#return (sum(45*pow(10, i, p) for i in range(k)) + sum(((i % 9) + 1)*pow(10, i // 9, p) for i in range(9*k, 9*k + m + 1)) - n - 1) % p
	#return (sum(45*pow(10, i, p) for i in range(k)) + sum((i + 1)*pow(10, k, p) for i in range(m + 1)) - n - 1) % p
	#return (45*sum(pow(10, i, p) for i in range(k)) + (m+2)*(m+1)//2*pow(10, k, p) - n - 1) % p
	#return (5*pow(10, k, p) + (m+2)*(m+1)//2*pow(10, k, p) - n - 6) % p
	return ((m*m + 3*m + 12)//2*pow(10, k % (p - 1), p) - n - 6) % p

print(S(20))

s = 0
f1, f2 = 1, 0
for n in range(2, 91):
	f1, f2 = f1 + f2, f1
	s = (s + S(f1)) % p
print(s)

