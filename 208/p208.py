from functools import lru_cache

@lru_cache(maxsize=None)
def ways(N, b, l_counts, r_counts, f):
	if all(c == 0 for c in l_counts) and all(c == 0 for c in r_counts):
		return 1
	res = 0
	if l_counts[f]:
		nl_counts = l_counts[:f] + (l_counts[f] - 1,) + l_counts[f+1:]
		res += ways(N, b, nl_counts, r_counts, (f + 1)%5)
	if r_counts[f]:
		nr_counts = r_counts[:f] + (r_counts[f] - 1,) + r_counts[f+1:]
		res += ways(N, b, l_counts, nr_counts, (f - 1)%5)
	return res

def Ways(N):
	if N%5:
		print("!!! N is not a multiple of 5")
		return 0
	tot = 0
	for b in range(N%2, N//5 + 1, 2):
		args = (N, b, ((N+5*b)//10,)*5, ((N-5*b)//10,)*5, 0)
		print("ways{}".format(args))
		res = ways(*args)
		ways.cache_clear()
		print(" --> {}".format(res))
		tot += 2*res if b else res
	return tot

print(Ways(70))

