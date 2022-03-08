from math import sqrt
from collections import Counter
from bisect import bisect_left

N = 30
J = (sqrt(17 + 8*N) - 3)//2

def add_part(partition, size):
	i = bisect_left(partition, (size, 1))
	if i == len(partition) or partition[i][0] != size:
		return partition[:i] + ((size, 1),) + partition[i:]
	return partition[:i] + ((size, partition[i][1] + 1),) + partition[i+1:]

def split_part(partition, i, j):
	s = bisect_left(partition, (j, 1), lo=0, hi=i)
	l = bisect_left(partition, (partition[i][0] - j - 1, 1), lo=s, hi=i)
	res = list(partition[:s])
	if partition[s][0] == j:
		res += [(j, partition[s][1] + 1), *partition[s+1:l]]
	else:
		res += [(j, 1), *partition[s:l]]
	if partition[l][0] == partition[i][0] - j - 1:
		res += [(partition[l][0], partition[l][1] + 1), *partition[l+1:i]]
	else:
		res += [(partition[i][0] - j - 1, 1), *partition[l:i]]
	if partition[i][1] == 1:
		return tuple(res) + partition[i+1:]
	return tuple(res) + ((partition[i][0], partition[i][1] - 1),) + partition[i+1:]

def shorten_part(partition, i):
	if partition[i][1] == 1:
		if i and partition[i-1][0] == partition[i][0] - 1:
			return partition[:i-1] + ((partition[i-1][0], partition[i-1][1] + 1),) + partition[i+1:]
		return partition[:i] + ((partition[i][0] - 1, 1),) + partition[i+1:]
	if i and partition[i-1][0] == partition[i][0] - 1:
		return partition[:i-1] + ((partition[i-1][0], partition[i-1][1] + 1), (partition[i][0], partition[i][1] - 1)) + partition[i+1:]
	return partition[:i] + ((partition[i][0] - 1, 1), (partition[i][0], partition[i][1] - 1)) + partition[i+1:]

def remove_part(partition, i):
	if partition[i][1] == 1:
		return partition[:i] + partition[i+1:]
	return partition[:i] + ((partition[i][0], partition[i][1] - 1),) + partition[i+1:]

def bisect_part(partition, i, half_size):
	j = bisect_left(partition, (half_size, 2), lo=0, hi=i)
	res = list(partition[:j])
	if partition[j][0] == half_size:
		res += [(half_size, partition[j][1] + 2), *partition[j+1:i]]
	else:
		res += [(half_size, 2), *partition[j:i]]
	if partition[i][1] == 1:
		return tuple(res) + partition[i+1:]
	return tuple(res) + ((partition[i][0], partition[i][1] - 1),) + partition[i+1:]

def next_ways(ways):
	n_ways = Counter()
	for (partition, left, right, m_there, m_max), count in ways.items():
		for i in range(left):#pick a number from the left block
			if left - i - 1:
				partition1 = add_part(partition, left - i - 1)
				m_here = m_there + 1
				m = max(m_here, m_max)
			else:
				partition1 = partition
				m_here = m_there
				m = m_max
			n_ways[(partition1, i, right, m_here, m)] += count
		for i in range(right):#pick a number from the right block
			if right - i - 1:
				partition1 = add_part(partition, right - i - 1)
				m_here = m_there + 1
				m = max(m_here, m_max)
			else:
				partition1 = partition
				m_here = m_there
				m = m_max
			n_ways[(partition1, left, i, m_here, m)] += count
		for i, (size, mult) in enumerate(partition):#for each partition
			for j in range(size//2):
				if j:
					partition1 = split_part(partition, i, j)
					m_here = m_there + 1
					m = max(m_here, m_max)
				else:
					partition1 = shorten_part(partition, i)
					m_here = m_there
					m = m_max
				n_ways[(partition1, left, right, m_here, m)] += 2*count*mult
			if size&1:
				if size == 1:
					partition1 = remove_part(partition, i)
					m_here = m_there - 1
					m = m_max
				else:
					partition1 = bisect_part(partition, i, size//2)
					m_here = m_there + 1
					m = max(m_here, m_max)
				n_ways[(partition1, left, right, m_here, m)] += count*mult
	return n_ways

ways = {((), i, N-i-1, 1, 1): 2 for i in range(N//2)}
if N&1:
	ways[((), N//2, N//2, 1, 1)] = 1
for t in range(1, N):
	print("{} current states\nQuantifying states with {} tiles".format(len(ways), t + 1))
	ways = next_ways(ways)

EM = {m: c for ((_, _, _, _, m), c) in ways.items()}
print(EM)

