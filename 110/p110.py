from math import log

def partitionfunc(n,k,l=1):
    '''n is the integer to partition, k is the length of partitions, l is the min partition element size'''
    if k < 1:
        raise StopIteration
    if k == 1:
        if n >= l:
            yield (n,)
        raise StopIteration
    for i in range(l,n+1):
        for result in partitionfunc(n-i,k-1,i):
            yield (i,)+result

p = [2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97]

def plsum(part):
	s = 0
	for i, n in enumerate(part):
		s += log(p[i])*n
	return s

def pprod(part):
	s = 1
	for i, n in enumerate(part):
		s *= p[i]**n
	return s

def dcount(part):
	c = 1
	for n in part:
		c *= n + 1
	return c

def dcountp(part, m):
	c = 1
	for n in part:
		c *= n*m + 1
	return c

minplsum = 35.46583382754037
minpart = (4,3,3,2,1,1,1,1,1,1,1)

for part in partitionfunc(19,11):
	part = part[::-1]
	partplsum = plsum(part)
	if dcountp(part, 2) <= 7999999:
		continue
	if partplsum < minplsum:
		minplsum = partplsum
		minpart = part

print(pprod(minpart))

