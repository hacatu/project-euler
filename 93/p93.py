import itertools

def s(a, b):
	return a + b
def d(a, b):
	return a - b
def p(a, b):
	return a*b
def q(a, b):
	return a/b
operatorSs = [
(s,s,s),(s,s,d),(s,s,p),(s,s,q),(s,d,s),(s,d,d),(s,d,p),(s,d,q),(s,p,s),(s,p,d),(s,p,p),(s,p,q),(s,q,s),(s,q,d),(s,q,p),(s,q,q),
(d,s,s),(d,s,d),(d,s,p),(d,s,q),(d,d,s),(d,d,d),(d,d,p),(d,d,q),(d,p,s),(d,p,d),(d,p,p),(d,p,q),(d,q,s),(d,q,d),(d,q,p),(d,q,q),
(p,s,s),(p,s,d),(p,s,p),(p,s,q),(p,d,s),(p,d,d),(p,d,p),(p,d,q),(p,p,s),(p,p,d),(p,p,p),(p,p,q),(p,q,s),(p,q,d),(p,q,p),(p,q,q),
(q,s,s),(q,s,d),(q,s,p),(q,s,q),(q,d,s),(q,d,d),(q,d,p),(q,d,q),(q,p,s),(q,p,d),(q,p,p),(q,p,q),(q,q,s),(q,q,d),(q,q,p),(q,q,q),
]

def addRight(p, o, r):
	try:
		addResult(o[2](p[0], o[1](p[1], o[0](p[2], p[3]))), r)
	except ZeroDivisionError:
		pass
def addRightx(p, o, r):
	try:
		addResult(o[2](p[0], o[1](o[0](p[1], p[2]), p[3])), r)
	except ZeroDivisionError:
		pass
def addQuad(p, o, r):
	try:
		addResult(o[2](o[1](p[0], p[1]), o[0](p[2], p[3])), r)
	except ZeroDivisionError:
		pass
def addLeftx(p, o, r):
	try:
		addResult(o[2](o[1](p[0], o[0](p[1], p[2])), p[3]), r)
	except ZeroDivisionError:
		pass
def addLeft(p, o, r):
	try:
		addResult(o[2](o[1](o[0](p[0], p[1]), p[2]), p[3]), r)
	except ZeroDivisionError:
		pass
def addResult(n, r):
	if n < 1 or n != int(n):
		return
	r.add(int(n))
def countConsec(s):
	r = 0
	for i in range(1, 3025):
		if i in s:
			r += 1
		else:
			break
	return r

maxCount = 0
maxS = 0
for combination in itertools.combinations([1,2,3,4,5,6,7,8,9], 4):
	results = set()
	for permutation in itertools.permutations(combination):
		for operatorS in operatorSs:
			addRight(permutation, operatorS, results)
			addRightx(permutation, operatorS, results)
			addQuad(permutation, operatorS, results)
			addLeftx(permutation, operatorS, results)
			addLeft(permutation, operatorS, results)
	count = countConsec(results)
	if count > maxCount:
		maxCount = count
		maxS = combination
print(maxS, maxCount)

