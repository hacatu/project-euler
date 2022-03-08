"""
Ok so on Project Euler you got a few levels of difficulty.
You got the gimmies, like the first 77, 165, 173, 174, 182, 225, 549
You got the standard problems, like most of the first 150 and most things from 200+
You got the neato burrito ones, like 112-117, 126, 191, 197, 199, 202, 208, 209, 213, 220,
233, 287, 317, 389, 435, 493, 527, 700, and 701
You got the toughies that take several days to a week and nearly double digit attempts like 152, 153, and 247
And then you start getting into the actually hard ones like 143, and 161
And the downright brutal ones like 163 and 177
And then
You got the mastapieces
The 100% difficulty bois
All problems at 85%+ difficulty look pretty much impossible to me, although the difficulty can be misleading because for instance
182 (RSA encryption) has a difficulty of 60% but is about as easy as problem number 1

This one, Counting Castles (502), has a special place in my heart.
I started project euler about 7 years ago on May 6, 2013 when I was 16.  I solved the first 100 problems
on March 22, 2015 when I finished problem 98.  After the first 150 I realized I would probably not be able to finish the problems
in order due to encountering 152.  I still haven't finished the first 200, or even 156.  On December 18, 2014, I solved the latest
problem (493, Under the Rainbow) for the first time.  I generally don't solve the most recent problems, instead focusing on
older ones.  A couple months later, though it feels like it's been there the whole time I've known about Project Euler, Counting
Castles was added.  This was the hardest problem on the site at the time, even among the very few other 100% difficulty problems.
Even now I don't know if I'll be able to handle it.  I regarded it as something I would never solve, a harbinger of the impossibility
of solving all the problems.  Now, Torpids and Cake Icing Puzzle exist and are somehow harder, but they don't have the same ring to the
name or appeal that Counting Castles does.

It seems like it might even be possible if I give in to hubris.  112-117, 163, and 701 give a tiny peek at some methods that seem to
peel away a tiny mote of the impossibility of the beheamoth.



So then, how can I count castles?

It is clear that unless I come up with some oracle tier god formula I will need to split the problem into three cases:
horizontal strips, vertical strips, and squaroids / squares, though these might as well be called medium, very hard, and very very hard.

The horizontal strip case actually seems solvable to me now.

In addition to F(w, h), we have an associated function G(w, h) counting the number of odd castles.
There are a couple key insights: F(w, h) is a bad function with bad properties because of the "h is reached" rule.  So we will generally
work with functions that compute F(w, <=h) and then F(w, h) can be recovered by F(w, <=h) - F(w, <= h-1).

Now the horizontal strip case is nice because f(w, h, r) = castles with width w, height at most h, and right column height r + 1.
We can keep a vector of f(w, h, r) and g(w, h, r) and then use a matrix to go to f(w+1, h, r) and g(w+1, h, r).  This requires a (2h+1)^2 matrix
be raised to the w power.

We could also think about f(w, h, l, r) and g(w, h, l, r) which might be helpful or necessary in other cases but is too complex for this case.
"""

import numpy as np
from numpy.linalg import matrix_power

"""
f(w+1,h,r) = \sum_{i=0}^{r-1}\begin[cases]f(w,h,i) & i = r \mod 2 \\ g(w, h, i) & i \not = \mod 2 \end[cases] + \sum_{i=r}^{h-1} f(w,h,i))
f(w,h,r) is stored at [r] and g(w,h,r) is stored at [h+r]
"""

def _mat_mulmod(A, B, p, c):
	C = np.zeros(A.shape, dtype=np.int64)
	n = A.shape[0]
	for i in range(n):
		for j in range(n):
			cij = 0
			for b in range(0, n, c):
				for k in range(b, min(b + c, n)):
					cij += A[i][k]*B[k][j]
				cij %= p
			C[i][j] = cij
	return C

def mat_powmod(M, n, p, c):
	S = M
	if not n&1:
		M = np.eye(M.shape[0], dtype=np.int64)
	S = _mat_mulmod(S, S, p, c)
	n >>= 1
	while n:
		print("(mat_powmod: n={})".format(n))
		if n&1:
			M = _mat_mulmod(M, S, p, c)
		S = _mat_mulmod(S, S, p, c)
		n >>= 1
	return M

def f_horiz(w, h, p):
	c = ((1<<63) - 1)//(p*p) - 1
	if c <= 1:
		raise ValueError("Modulus too large, can't prevent overflow")
	
	dW = [[0]*(2*h) for _ in range(2*h)]
	
	for r in range(h):
		for i in range(r):
			if (i&1) == (r&1):
				dW[r][i] = 1     #f(w+1, h, r) = ... + f(w, h, i) + ... if i = r mod 2
				dW[r+h][i+h] = 1 #g(w+1, h, r) = ... + g(w, h, i) + ... if i = r mod 2
			else:
				dW[r][i+h] = 1   #f(w+1, h, r) = ... + g(w, h, i) + ... if i != r mod 2
				dW[r+h][i] = 1   #g(w+1, h, r) = ... + f(w, h, i) + ... if i != r mod 2
		for i in range(r, h):
			dW[r][i] = 1         #f(w+1, h, r) = ... + f(w, h, i) + ...
			dW[r+h][i+h] = 1     #g(w+1, h, r) = ... + g(w, h, i) + ...
	
	dW = np.array(dW, dtype=np.int64)
	Fr = np.array([0]*h + [1] + [0]*(h-1), dtype=np.int64)
	#W = matrix_power(dW, w)   #disney: where the magic happens
	W = mat_powmod(dW, w, p, c)   #disney: where the magic happens
	#return sum(int(W[r][h]) for r in range(h))
	res = 0
	for b in range(0, h, c):
		for r in range(b, min(b + c, h)):
			res += W[r][h]
		res %= p
	return int(res)%p

def F_horiz(w, h, p):
	return f_horiz(w, h, p) - f_horiz(w, h - 1, p)

