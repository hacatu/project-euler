from itertools import permutations, count
from string import ascii_lowercase
from collections import deque

#from https://stackoverflow.com/a/34404546/2171981
def ilen(it):
	counter = count()
	deque(zip(it, counter), 0)
	return next(counter)

def nCr(n, r):
	ret = 1
	for a, b in zip(range(n - r + 1, n + 1), range(1, r + 1)):
		ret = ret*a//b
	return ret

def p_brute(n):
	def is_valid(s):
		inc = False
		for i in range(1, len(s)):
			if s[i-1] < s[i]:
				if inc:
					return False
				inc = True
		return inc
	
	return ilen(s for s in map(''.join, permutations(ascii_lowercase, r=n)) if is_valid(s))

def p_closed(n):
	return nCr(26, n)*((1 << n) - n - 1)

	