import resource, sys

resource.setrlimit(resource.RLIMIT_STACK, (resource.RLIM_INFINITY, resource.RLIM_INFINITY))

sys.setrecursionlimit(61000)

MAX = 60000

partitions = [-1]*(MAX + 1)
partitions[0] = 1

def partition(n):
	if n < 0:
		return 0
	if partitions[n] == -1:
		partitions[n] = 0
		k = 1
		while True:
			if n < k*(3*k - 1)//2:
				break
			a = partition(n - (k*(3*k - 1))//2)
			if k%2 == 0:
				partitions[n] -= a
			else:
				partitions[n] += a
			if n < k*(3*k + 1)//2:
				break
			a = partition(n - (k*(3*k + 1))//2)
			if k%2 == 0:
				partitions[n] -= a
			else:
				partitions[n] += a
			k += 1
			partitions[n] %= 1000000
	return partitions[n]

n = 1
while partition(n):
	n += 1
print(n)
