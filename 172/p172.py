"""
[t**18] (x_1+...+x_9)*(x_0+...+x_9)**17 mod x_0**4,...,x_9**4
[t**18] (x_1+...+x_9)*sum(k_0=0...3;(17:k_0)*x_0**(k_0)*(x_1+...+x_9)**(17-k_0)) mod x_0**4,...,x_9**4
[t**18] sum(k_0=0...3;(17:k_0)*x_0**(k_0)*(x_1+...+x_9)**(18-K_0)) mod x_1**4,...,x_9**4
[t**18] sum(k_0=0...3;(17:k_0)*x_0**(k_0)*sum(k_1=0...3;(18-K_0:k_1)*x_1**(k_1)*(x_2+...+x_0)**(18-K_1))) mod x_2**4,...,x_9**4
...
[t**18] sum(k_0=0...3;(17:k_0)*x_0**(k_0)*sum(k_1=0...3;(18-K_0:k_1)*x_1**(k_1)*sum(...sum(k_8=0...3;(18-K_7:k_8)*x_8**(k_8)*x_9**(18-K_8))...)))

sum(k_0=0...3;(17:k_0)*sum(k_1=0...3;(18-K_0:k_1)*sum(...sum(k_8=0...3;(18-K_7:k_8))...)))

0 <= 18-K_8 <= 3
15 <= K_8 <= 18
12 <= K_7 <= 18
...
-9 <= K_0 <= 18

3*(i-3) <= K + k < 19
"""
_ncr = [[1] + [0]*18 for _ in range(19)]
for i in range(1, 19):
	_ncr[i][0] = 1
	for j in range(1, 19):
		_ncr[i][j] = _ncr[i-1][j-1] + _ncr[i-1][j]

def ncr(n, r):
	return _ncr[n][r]

def sum_rec(i, K):
	if i == 8:
		acc = 0
		for k in range(max(0, 3*(i - 3) - K), min(4, 19 - K)):
			acc += ncr(18 - K, k)
		return acc
	acc = 0
	for k in range(max(0, 3*(i - 3) - K), min(4, 19 - K)):
		acc += ncr(18 - K, k)*sum_rec(i + 1, K + k)
	return acc

def sum_head():
	acc = 0
	for k in range(0, 4):
		acc += ncr(17, k)*sum_rec(1, k)
	return acc

print(sum_head())

