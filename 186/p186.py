def LFG():
	st = [None]*55
	for k in range(1,56):
		st[k%55] = (100003 + (300007*k*k - 200003)*k)%1000000
		yield st[k%55]
	k = 56
	while True:
		st[k%55] = (st[(k - 24)%55] + st[k%55])%1000000
		yield st[k%55]
		k += 1

groups = [{i} for i in range(1000000)]

lfg = iter(LFG())

calls = 0

while len(groups[524287]) < 990000:
	#print(len(groups[524287]))
	a = next(lfg)
	b = next(lfg)
	if a == b:
		continue
	calls += 1
	if groups[a] is not groups[b]:
		if len(groups[a]) < len(groups[b]):
			a, b = b, a
		g = groups[b]
		for c in g:
			groups[c] = groups[a]
		groups[a] |= g

print(calls)

