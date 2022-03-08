def memoize(f):
	memo = {};
	def helper(x):
		if x not in memo:
			memo[x] = f(x)
		return memo[x]
	return helper

@memoize
def f(x):
	if x == 0:
		return 1
	if x%2 == 1:
		return f((x - 1)//2)
	return f(x//2) + f(x//2 - 1)

print(f(10**25))

