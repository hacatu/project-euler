from bitarray import bitarray
from collections import Counter

N = 500
visited = bitarray(N)
visited.setall(False)
odd_chains = Counter()
single_bed_even_chains = Counter()
single_desk_even_chains = Counter()
cycles = Counter()

def parse_file(f):
	res = {}
	for line in f.readlines():
		p = line.split(",", 1)
		if len(p) == 1:
			break
		i, j = map(int, p)
		res[i] = j
		res[j] = i
	return res

with open("beds.txt", "r") as beds_f:
	roomates = parse_file(beds_f)

with open("desks.txt", "r") as desks_f:
	deskmates = parse_file(desks_f)

for i in range(1, N + 1):
	if visited[i - 1]:
		continue
	reachable = 1
	left = i
	singles = 0
	while True:
		tmp = roomates.get(left)
		if not tmp:
			break
		reachable += 1
		left = tmp
		singles = 1
		visited[left - 1] = True
		tmp = deskmates.get(left)
		if not tmp:
			break
		left = tmp
		if left == i:
			break
		reachable += 1
		singles = 0
		visited[left - 1] = True
	if left == i and reachable > 1:
		cycles[reachable] += 1
		continue
	right = i
	while True:
		tmp = deskmates.get(right)
		if not tmp:
			break
		right = tmp
		if left == right:
			break
		reachable += 1
		visited[right - 1] = True
		tmp = roomates.get(right)
		if not tmp:
			break
		right = tmp
		if left == right:
			break
		reachable += 1
		visited[right - 1] = True
	if reachable&1:
		odd_chains[reachable] += 1
	elif right == left:
		cycles[reachable] += 1
	elif singles:
		single_desk_even_chains[reachable] += 1
	else:
		single_bed_even_chains[reachable] += 1

ways = 1
MOD = 999999937

factorial = list(range(N + 1))
factorial[0] = 1
for n in range(1, N + 1):
	factorial[n] = n*factorial[n - 1]%MOD

for size, count in odd_chains.items():
	print("{}-chains (mixed ends): {}".format(size, count))
	ways = ways*factorial[count]%MOD

for size, count in single_bed_even_chains.items():
	print("{}-chains (single bed ends): {}".format(size, count))
	ways = ways*factorial[count]*2**count%MOD

for size, count in single_desk_even_chains.items():
	print("{}-chains (single desk ends): {}".format(size, count))
	ways = ways*factorial[count]*2**count%MOD

for size, count in cycles.items():
	print("{}-cycles: {}".format(size, count))
	ways = ways*factorial[count]*pow(size, count, MOD)%MOD

print("Agreeable permutations: {}".format(ways))

