from itertools import permutations

def max_order_value(chunks):
	chunks.sort(key=lambda product: product[0], reverse=True)
	ret = 0
	for product in chunks:
		ret = ret*product[1] + product[2]
	return ret

def max_solution_rec(products, chunks, length, max_length, mask, max_solution):
	if length == 0:
		return max(max_solution, max_order_value(chunks))
	for l in range(1, min(length, max_length) + 1):
		for product in products[l - 1]:
			if (product[3] & mask) != 0:
				continue
			chunks.append(product)
			max_solution = max_solution_rec(products, chunks, length - l, l, mask | product[3], max_solution)
			chunks.pop()
	return max_solution

max_concat = 0

for coeff_len in range(1, 6):
	print(coeff_len)
	for coeff_tuple in permutations(range(0, 10), coeff_len):
		if coeff_tuple[0] == 0:
			continue
		products = []
		coeff = 0
		for d in coeff_tuple:
			coeff = coeff*10 + d
		other_digit_set = set(range(0, 10)) - set(coeff_tuple)
		for other_len in range(11 - coeff_len):
			products += [[]]
			for other_tuple in permutations(other_digit_set, other_len):
				other = 0
				inMask = 0
				for d in other_tuple:
					other = other*10 + d
					inMask += 1 << d
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
				if tmp != 0:
					continue
				digitMask = (outMask << 10) + inMask
				t = 10**product_len
				key = product*10**(10 - product_len)
				products[-1] += [(key, t, product, digitMask)]
		max_concat = max_solution_rec(products, [], 10 - coeff_len, 10 - coeff_len, 0, max_concat)
print(max_concat)

