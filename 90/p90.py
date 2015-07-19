from itertools import combinations
pairs = [(0,1),(0,4),(0,6),(1,6),(2,5),(3,6),(4,6),(1,8)]
def f9(n):
	return 6 if n == 9 else n
def checkPair(a, b, pair):
	x, y = pair
	return (x in a and y in b) or (y in a and x in b)
def check(a, b):
	return all([checkPair(a, b, pair) for pair in pairs])
c = 0
for a in combinations(range(10), 6):
	for b in combinations(range(10), 6):
		if check([f9(n) for n in a], [f9(n) for n in b]):
			c += 1;
print(c//2)
