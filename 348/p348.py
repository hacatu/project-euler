import heapq
import itertools

def square_cube_sums():
	generators = [(2**2 + 2**3, 2, 2)]
	a_gen = (2**2 + 3**3, 2, 3)
	b_gen = (3**2 + 2**3, 3, 2)
	while True:
		if a_gen[0] <= generators[0][0]:
			heapq.heappush(generators, a_gen)
			_, a, b = a_gen
			b += 1
			a_gen = (a**2 + b**3, a, b)
		if b_gen[0] <= generators[0][0]:
			heapq.heappush(generators, b_gen)
			_, a, b = b_gen
			a += 1
			b_gen = (a**2 + b**3, a, b)
		yield generators[0][0]
		_, a, b = generators[0]
		a += 1
		b += 1
		generators[0] = (a**2 + b**3, a, b)
		heapq._siftup(generators, 0)

def is_palindrome(n):
	k = n
	r = 0
	while k != 0:
		r = r*10 + k%10
		k //= 10
	return r == n

print(sum(itertools.islice((n for n, g in itertools.groupby(square_cube_sums()) if len(list(g)) == 4 and is_palindrome(n)), 5)))

