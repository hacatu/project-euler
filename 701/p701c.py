from frozendict import frozendict
from collections import defaultdict

"""
The states for the pdf are mappings from subsets of the columns to regions, where each region has a size,
together with the size of the largest detached region.

Obviously, the bitmasks indicating the subsets for two different regions must not just be disjoint but
have at least one common zero between disjoint ones.

This means there can be at most ceil(W/2) regions, and each one can have at most WH - W + 2 - ceil(W/2)
cells.

The pdf (really a counting function) at row 0 can be trivially given as {{0: 0}: 1}

Then for every row, every bit string of length W is equally likely and from this we can "easily" tabulate the pdf after the row
"""

W, H = 7, 7

def bitcount(x):
	b = 0
	while x > 0:
		x &= x - 1
		b += 1
	return b

def ctz(v):
	return (v&-v).bit_length() - 1

def row_groups(r_mask):
	res = []
	while r_mask:
		b = ctz(r_mask)
		lo_mask = ((-1) << b)
		a = ctz(~r_mask & lo_mask)
		hi_mask = ((-1) << a)
		res.append(r_mask & ~hi_mask)
		r_mask &= hi_mask
	return res

def next_pdf(pdf):
	n_pdf = defaultdict(int)
	for _case, count in pdf.items():
		#print(" case: {}x{}".format(_case, count))
		for r_mask in range(1 << W):
			case = dict(_case)
			#print("  r_mask: {}".format(r_mask))
			r_groups = row_groups(r_mask)
			n_case = {}
			while r_groups:
				g_mask = r_groups.pop()
				total = 0
				l_mask = 0
				while True:
					matches = []
					for group, size in case.items():
						if group & g_mask:
							matches.append(group)
							total += size
							l_mask |= group
					if not matches:
						break
					for group in matches:
						del case[group]
					j = len(r_groups) - 1
					for i in range(j, -1, -1):
						if r_groups[i] & l_mask:
							g_mask |= r_groups[i]
							r_groups[i] = r_groups[j]
							j -= 1
					if j == len(r_groups) - 1:
						break
					del r_groups[j+1:]
				total += bitcount(g_mask)
				n_case[g_mask] = total
			n_case[0] = max(case.values())
			#print("  n_case[0]: {}".format(n_case[0]))
			n_case = frozendict(n_case)
			#print("  +n_case: {}x{}".format(n_case, count))
			n_pdf[n_case] += count
	return n_pdf

def expected_value(pdf):
	return sum(max(case.values())*count for (case, count) in pdf.items())/sum(pdf.values())

pdf = defaultdict(int, [(frozendict({0: 0}), 1)])
for r in range(H):
	#print(pdf)
	print("Adding row {}".format(r))
	pdf = next_pdf(pdf)
#print(pdf)
print("E(m)={}".format(expected_value(pdf)))

