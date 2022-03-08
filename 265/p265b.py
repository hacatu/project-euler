def debruijn_sequences_rec(n, head, subs, j, tail):
	if j == 1 << n:
		for d in head[:n - 1]:
			tail = ((tail << 1) & ((1 << n) - 1)) | d
			if subs[tail]:
				return
		yield head
		return
	for d in [0, 1]:
		new_tail = ((tail << 1) & ((1 << n) - 1)) | d
		if not subs[new_tail]:
			subs[new_tail] = 1
			head[j] = d
			yield from debruijn_sequences_rec(n, head, subs, j + 1, new_tail)
			subs[new_tail] = 0

def debruijn_sequences(n):
	head = [0]*(1 << n)
	subs = [1] + [0]*((1 << n) - 1)
	j = n
	tail = 0
	yield from debruijn_sequences_rec(n, head, subs, j, tail)

def s(n):
	r = 0
	for a in debruijn_sequences(n):
		t = 0
		for d in a[n:]:
			t = (t << 1) | d
		r += t
	return r

def is_linear(a, n):
	tail = 0
	im = [0]*n
	for j in range(n, 1 << n - 1):
		d = a[j]
		tail = ((tail << 1) & ((1 << n) - 1)) | d
		if tail & (tail - 1) == 0:
			im[tail.bit_length() - 1] = a[j + 1]
	tail = ((tail << 1) & ((1 << n) - 1)) | 1
	for j in range(n - 1):
		d = a[j]
		tail = ((tail << 1) & ((1 << n) - 1)) | d
		if tail & (tail - 1) == 0:
			im[tail.bit_length() - 1] = a[j + 1]
	tail = 0
	for j in range(n, 1 << n - 1):
		d = a[j]
		tail = ((tail << 1) & ((1 << n) - 1)) | d
		y = 0
		for b in range(n):
			if (tail >> n) & 1:
				y ^= im[b]
		if y != a[j + 1]:
			return False
	return True

for a in debruijn_sequences(3):
	print(a)
	print("is linear" if is_linear(a, 3) else "is not linear")

