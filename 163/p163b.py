"""
There are six kinds of triangles, associated with the six distinct ways to choose 3 of the 6
directions of lines to intersect.  Call the orthagonal lines ABC and the diagonal lines abc,
where a is perpendicular to A, etc.

#ABC triangles
#Abc, Nca, Cab triangles
Aac, Aab, Bba, Bbc, Cca, Ccb triangles
#AaC, AaB, BbA, BbC, CcA, CcB triangles
#ABc, BCa, CAb triangles
#abc triangles

N = number of triangles per side

ABC:
N^2

abc:
N*(N-1)/2

Abc:
This is the simplest one I think.
Edit: it isn't
There are N A lines, one of length N, one of length (N-1), and so on.  For the lines from 1
to floor(3N/4), this can happen on both sides.
For lines from floor(3N/4)+1 to N, not all flipped triangles are possible.
In particular, the width limit for line N-k is 3k but line N-k is N-k triangles wide.
N-k <= 3k -> N <= 4k -> N/4 <= k this is where the floor(3N/4) comes from.
So the number of triangles of type Abc is
6*\sum_{n=1}^\floor{\frac{3N}{4}}\binom{n+1}{2} +
3*\sum_{n=\floor{\frac{3N}{4}}+1}^{N-1}\sum_{w=1}^{3(N-n)}(n+1-w) +
3*\sum_{n=\floor{\frac{3N}{4}}+1}^N\binom{n+1}{2}
This is a cubic polynomial and the overall probably is too

ABc:
This is similar to the Abc case.  There are 2*N - 1 vertical lines.  N of them start in the
middle of upward facing triangles, and N - 1 start between them.  The first and last ones in
the middle of triangles will have no ABc triangles.

If N is even, the central vertical line is between upward facing triangles.  There are (N - 2)/2
intertriangular vertical lines on each side, 1 in the middle, and (N - 2)/2 intratriangular
vertical lines on each side.
12*\sum_{n=1}^{(N - 2)/2}\binom{n+1}{2} +
12*\sum_{n=1}^{(N - 2)/2}\binom{n+1}{2} +
6*\binom{(N + 2)/2}{2}

If N is odd, the central vertical line is in the middle of an upward facing triangle.  There
are (N - 3)/2 intratriangular vertical lines on each side, 1 in the middle, and
(N - 1)/2 intertriangular vertical lines on each side.
12*\sum_{n=1}^{(N - 3)/2}\binom{n+1}{2} +
12*\sum_{n=1}^{(N - 1)/2}\binom{n+1}{2} +
6*\binom{(N + 1)/2}{2}

AaC:
          |\
          | \
        __|__\
      |\  |   |\
      | \ |   | \
    __|__\|___|__\
  |\  |   |\  |   |\
  | \ |   | \ |   | \
__|__\|___|__\|___|__\

This is a pretty chunky one.  Let's consider the horizontal lines as the outermost thing to
sum over.  That is, for each horizontal line, how many Aac triangles are there including it?
There are as always two orientations, those with the horizontal line on the bottom (upward)
and with it on the bottom (downward).  First, the upward triangles.  There are tons of ways
to conceptualize them.  For each negative orthagonal, any horizontal and vertical pair
intersecting it works.
For downward triangles, we face a similar situation to the reverse triangles in the Abc case.
In the negative orthagonals from 1 to N, the number of single triangles in line n is n.
However, N can accomidate width 0 downward triangles, N-1 can accomidate width 2 triangles,
and in general N-k can accomidate 2k width triangles.  As long as N-k<=2k, the N-k limit
is overruling.  Otherwise, the 2k limit is overruling.  N<=3k
6*\sum_{n=1}^N\binom{n+1}{2} +
6*\sum_{n=1}^\floor{\frac{2N}{3}}\binom{n+1}{2} +
6*\sum_{n=\floor{\frac{2N}{3}}+1}^{N-1}\sum_{w=1}^{2(N-n)}(n+1-w)

Aac:
Consider A as the horizontal orthagonals, a as the vertical diagonals, and c as the negative
diagonals.

Now upward pointing triangles are relatively easy, we look at the negative diagonals.
There are N-1 negative diagonals that intersect the left side of the outer triangle between
small triangles, call these even, and N that intersect in small triangles, call these odd.

Even negative diagonals have 3n segments for some n whereas odd negative diagonals have
3n-2.  Vertical diagonals come at every segment but horizontal orthagonals only come at
every third segment.  So for the nth even diagonal there are n options for the horizontal
orthagonal and 3 times the number of horizontal orthagonals down we go choices for
the vertical diagonal.  If N is odd, N-1 (the number of even diagonals) is even so the number
of upward pointing triangles on even negative diagonals is
12*\sum_{n=1}^{(N-1)/2}\sum_{k=1}^{n}3k
If N is even it is
12*\sum_{n=1}^{(N-2)/2}\sum_{k=1}^{n}3k + 6*\sum_{k=1}^{N/2}3k

For the odd diagonals, if N is even,
12*\sum_{n=1}^{N/2}\sum_{k=1}^{n}3k-2
If N is odd,
12*\sum_{n=1}^{(N-1)/2}\sum_{k=1}^{n}3k-2 + 6*\sum_{k=1}^{(N+1)/2}3k-2

Finally, we have to consider the downward triangles.  We will base these off of the
horizontal orthagonals, so the maximum height is restricted by both the width at that
horizontal orthagonal and the height below it.  In particular, the width bound for
1,...,n counting from the top is 2n and the height below bound for N-k is 3k.
2n <= 3k, n+k=N, k = N-n, 2n <= 3(N-n), 5n <= 3N, n <= floor(3N/5)

The negative diagonals come in at every other segment along the horizontal orthagonal.
First, before the height below bound comes into effect, we have
6*\sum_{n=1}^{floor(3N/5)}\sum_{k=1}^{n}2k
But once the height below bound comes into effect, things get trickier because the rightmost
negative diagonals won't actually have height below bounds.
6*\sum_{n=floor(3N/5)+1}^{N-1}\sum_{g=1}^{n}min(2g,N-n)
=6*\sum_{n=floor(3N/5)+1}^{N-1}(\sum_{g<floor((N-n)/2)}2g + \sum_{g=floor((N-n)/2)}^{n}(N-n))
=6*\sum_{n=floor(3N/5)+1}^{N-1}(floor((N-n)/2)(floor((N-n)/2)+1) + (n+1-floor((N-n)/2))(N-n))
"""
from sympy import symbols, binomial, Sum, Piecewise, Mod, Eq, floor, Min

def is_even(n):
	return Eq(Mod(n, 2), 0)

def is_odd(n):
	return Eq(Mod(n, 2), 1)

N, n, k, w, g = symbols("N n k w g", integer=True, nonnegative=True)

ABC = floor(N*(N+2)*(2*N+1)/8)
abc_right_inter_half = Sum(Sum(Min(floor((3*N-3*n-b)/3), 3*n-b), (b, 0, 3*n-1)), n=1, floor((N-1)/2))
abc_right_intra_half = Sum(Sum(Min(floor((3*N-3*n-b-1)/3), 3*n-3-b), (b, 0, 3*n-3)), n=1, floor((N-1)/2))
abc_left_inter_half = Sum(Sum(k, (k, 1, floor(3*n))) + Sum(3*n + 1 - k, (k, floor(3*n) + 1, 3*n)), (n, 1, floor(N/2)-1))
abc_left_intra_half = Sum(Sum(k, (k, 1, floor(3*n))) + Sum(3*n + 1 - k, (k, floor(3*n) + 1, 3*n)), (n, 1, floor(N/2)))
abc = 2*(abc_right_inter_half + abc_right_intra_half + abc_left_inter_half + abc_right_intra_half)
Abc = (6*Sum(binomial(n+1,2), (n,1,floor(3*N/4))) +
3*Sum(Sum(n+1-w, (w,1,3*(N-n))), (n,floor(3*N/4)+1,N-1)) +
3*Sum(binomial(n+1,2), (n,floor(3*N/4)+1,N)))
ABc = Piecewise((
	12*Sum(binomial(n+1,2), (n,1,(N - 2)/2)) +
	12*Sum(binomial(n+1,2), (n,1,(N - 2)/2)) +
	6*binomial((N + 2)/2,2), is_even(N)
), (
	12*Sum(binomial(n+1,2), (n,1,(N - 3)/2)) +
	12*Sum(binomial(n+1,2), (n,1,(N - 1)/2)) +
	6*binomial((N + 1)/2,2), is_odd(N)
))
AaC = (6*Sum(binomial(n+1,2), (n,1,N)) +
6*Sum(binomial(n+1,2), (n,1,floor(2*N/3))) +
6*Sum(Sum(n+1-w, (w,1,2*(N-n))), (n,floor(2*N/3)+1,N-1)))
Aac = (Piecewise((
	12*Sum(Sum(3*k, (k,1,n)), (n,1,(N-2)/2)) +
	12*Sum(Sum(3*k-2, (k,1,n)), (n,1,N/2)) +
	6*Sum(3*k, (k,1,N/2)), is_even(N)
), (
	12*Sum(Sum(3*k, (k,1,n)), (n,1,(N-1)/2)) +
	12*Sum(Sum(3*k-2, (k,1,n)), (n,1,(N-1)/2)) +
	6*Sum(3*k-2, (k,1,(N+1)/2)), is_odd(N)
)) +
6*Sum(floor((N-n)/2)*(floor((N-n)/2)+1) + (n+1-floor((N-n)/2))*(N-n), (n,floor(3*N/5)+1,N-1)))

"""
sympy cannot simplify

Sum(floor((N-n)/2)*(floor((N-n)/2)+1) + (n+1-floor((N-n)/2))*(N-n), (n,floor(3*N/5)+1,N-1)))

on its own.  We can make the floors drop out by putting it in a piecewise function on the parity of
N and more importantly splitting it into two sums, one where n = 2k+1 is odd and one where n = 2k is
even.

floor(3*N/5) + 1 <= 2*k <= N - 1
(floor(3*N/5) + 1)/2 <= k <= (N - 1)/2, but k is an integer so we can ceil the lhs and floor the rhs
ceil((floor(3*N/5) + 1)/2) <= k <= floor((N - 1)/2), rewrite ceil as floor by adding denom - 1 to num
floor((floor(3*N/5) + 2)/2) <= k <= floor((N - 1)/2), simplify nested floor
floor(3*N/10) + 1 <= k <= floor((N - 1)/2)

Similarly for odd n,
floor(3*N/5) + 1 <= 2*k + 1 <= N - 1, can be rewritten
floor((3*N + 5)/10) <= k <= floor(N/2) - 1

Meaning the sum can be rewritten
Sum(floor((N-2*k)/2)*(floor((N-2*k)/2)+1) + (2*k+1-floor((N-2*k)/2))*(N-2*k), (k,floor(3*N/10)+1,floor((N-1)/2)) +
Sum(floor((N-2*k+1)/2)*(floor((N-2*k+1)/2)+1) + (n+1-floor((N-2*k+1)/2))*(N-2*k+1), (k,floor((3*N+5)/10),floor(N/2)-1)))
"""

Aac_alt = (Piecewise((
	12*Sum(Sum(3*k, (k,1,n)), (n,1,(N-2)/2)) +
	12*Sum(Sum(3*k-2, (k,1,n)), (n,1,N/2)) +
	6*Sum(3*k, (k,1,N/2)), is_even(N)
), (
	12*Sum(Sum(3*k, (k,1,n)), (n,1,(N-1)/2)) +
	12*Sum(Sum(3*k-2, (k,1,n)), (n,1,(N-1)/2)) +
	6*Sum(3*k-2, (k,1,(N+1)/2)), is_odd(N)
)) + 6*Piecewise((
	Sum(floor((N-2*k)/2)*(floor((N-2*k)/2)+1) + (2*k+1-floor((N-2*k)/2))*(N-2*k), (k,floor(3*N/10)+1,floor((N-1)/2))) +
	Sum(floor((N-2*k+1)/2)*(floor((N-2*k+1)/2)+1) + (2*k+2-floor((N-2*k+1)/2))*(N-2*k+1), (k,floor((3*N+5)/10),floor(N/2)-1)),
	is_even(N)
), (
	Sum(floor((N-2*k)/2)*(floor((N-2*k)/2)+1) + (2*k+1-floor((N-2*k)/2))*(N-2*k), (k,floor(3*N/10)+1,floor((N-1)/2))) +
	Sum(floor((N-2*k+1)/2)*(floor((N-2*k+1)/2)+1) + (2*k+2-floor((N-2*k+1)/2))*(N-2*k+1), (k,floor((3*N+5)/10),floor(N/2)-1)),
	is_odd(N)
)))

triang = ABC + abc + Abc + ABc + AaC + Aac

