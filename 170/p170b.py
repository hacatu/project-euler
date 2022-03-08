from itertools import permutations

def max_process(products, permutation, partition, max_concat):
	i = 0
	a = 0
	mask = 0
	chunks = []
	for part in partition:
		chunk = permutation[a : a + part]
		value = 0
		for d in chunk:
			value = value*10 + d
		product = products[value]
		if (product[3] & mask) != 0:
			return max_concat
		mask |= product[3]
		chunks.append(product)
		a += part
		i += 1
	chunks.sort(key=lambda t:t[0], reverse=True)
	value = 0
	for chunk in chunks:
		value = value*chunk[2] + chunk[1]
	return max(value, max_concat)

def make_partitions_wrapped(length, max_len):
	if make_partitions[length - 1][max_len - 1] is None:
		make_partitions[length - 1][max_len - 1] = []
		for l in range(1, min(length, max_len) + 1):
			for part in make_partitions_wrapped(length - l, min(length - l, l)):
				make_partitions[length - 1][max_len - 1].append(part + [l])
	return make_partitions[length - 1][max_len - 1]

make_partitions = []

for length in range(1, 10):
	make_partitions.append([None]*length)

for length in range(1, 10):
	for max_len in range(1, length + 1):
		make_partitions_wrapped(length, max_len)

max_concat = 0

for coeff_len in range(1, 6):
	print(coeff_len)
	for coeff_tuple in permutations(range(0, 10), coeff_len):
		if coeff_tuple[0] == 0:
			continue
		products = {}
		coeff = 0
		for d in coeff_tuple:
			coeff = coeff*10 + d
		other_digit_set = set(range(0, 10)) - set(coeff_tuple)
		for other_len in range(11 - coeff_len):
			for other_tuple in permutations(other_digit_set, other_len):
				other = 0
				for d in other_tuple:
					other = other*10 + d
				product = coeff*other
				outMask = 0
				product_len = 0
				tmp = product
				while tmp != 0:
					if coeff != 1 and (outMask & (1 << (tmp%10))) != 0:
						break
					outMask += 1 << (tmp%10)
					tmp //= 10
					product_len += 1
				if tmp != 0 or product_len > other_len + 1:
					continue
				t = 10**product_len
				key = product*10**(10 - product_len)
				products[product] = (key, product, t, outMask)
		partitions = make_partitions_wrapped(10 - coeff_len, 10 - coeff_len)
		for permutation in permutations(other_digit_set):
			for partition in partitions:
				max_concat = max_process(products, permutation, partition, max_concat)
			
print(max_concat)

