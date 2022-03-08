"""
Product       Supplier
			  A
1 Cavier      5248 = 2^7.41        640 = 2^7.5
2 Cake        1312 = 2^5.41       1888 = 2^5.59
3 Joint       2624 = 2^6.41       3776 = 2^6.59
4 Wine        5760 = 2^7.3^2.5    3776 = 2^6.59
5 Truffles    3936 = 2^5.3.41     5664 = 2^5.3.59

  Sum        18880 = 2^6.5.59    15744 = 2^7.3.41

b_i*A_i/(B_i*a_i) = p/q = m = sum(a_i)*sum(B_i)/(sum(b_i)*sum(A_i))

b_i = m*a_i*B_i/A_i

sum(a_i)*sum(B_i)/(sum(A_i)*sum(m*a_i*B_i/A_i)) = m
sum(a_i)*sum(B_i)/(sum(A_i)*sum(a_i*B_i/A_i)) = m^2

sum(A_i)/sum(B_i)*sum(B_i/A_i*a_i)/sum(a_i) = m^2
5*59/(2*3*41)*(5/41*a_1 + 59/41*a_2 + 59/41*a_3 + 59/90*a_4 + 59/41*a_5)/sum(a_i) = 1/m^2
(5/41*a_1 + 59/41*a_2 + 59/41*a_3 + 59/90*a_4 + 59/41*a_5)/sum(a_i) = 2*3*41/(5*59*m^2)
59/41 + ((5/41 - 59/41)*a_1 + (59/90 - 59/41)*a_4)/sum(a_i) = 2*3*41/(5*59*m^2)
59/41 - (54/41*a_1 + (49*59/(41*90))*a_4)/sum(a_i) = 2*3*41/(5*59*m^2)
90*59 - 90*54 < 90*59 - (90*54*a_1 + 49*59*a_4)/sum(a_i) = 6*90*41^2/(5*59*m^2)
90*5 < 6*90*41^2/(5*59*m^2)
5^2*59*m^2 < 6*41^2
m < 41/5*sqrt(6/59)

Call this upper bound M

Also, q | gcd(B_i*a_i) and A_i*q | p*a_i*B_i.
Let d = gcd(a_i) and a_i = l_i*d.

This gives us
41q | 5p*d*l_1
41q | 59p*d*l_2
41q | 59p*d*l_3
90q | 59p*d*l_4
41q | 59p*d*l_5

So in particular q | 5*59*p*d, and if we assume p/q is reduced we have
q | 5*59*d

This gets another bound, since d <= A_i so q <= 5*59*d <= 5*59*1312,
so 1 <= q <= 5*59*1312 and p/q is reduced with 1 < p/q < 41/5*sqrt(6/59).

By taking the continued fraction expansion of M, we get m <= 133909/51209,
so 1476/1475 <= p/q <= 133909/51209 with p/q reduced and q <= 387040
"""

from fractions import Fraction as Q

def cfrac(cs):
	f = cs[-1]
	n = Q(1,1)
	for c in cs[-2::-1]:
		f = c + n/f
	return f

cs = [2,1,1,1,1,2,13,4,14,4,1,83]

class SternBrocotTreeProvider:
	def __init__(self, a, b, c, d, denom):
		self.a = a
		self.b = b
		self.c = c
		self.d = d
		self.denom = denom
	
	def traversePreorder(self, rationalVisitor):
		p = self.a + self.c
		q = self.b + self.d
		if q > self.denom:
			return
		go_left, go_right = rationalVisitor(p, q)
		if go_left:
			SternBrocotTreeProvider(self.a, self.b, p, q, self.denom).traversePreorder(rationalVisitor)
		if go_right:
			SternBrocotTreeProvider(p, q, self.c, self.d, self.denom).traversePreorder(rationalVisitor)

def logRational(a, b):
	left_ord = 1475*a - 1476*b
	go_left = left_ord > 0
	right_ord = 133909*b - 51209*a
	go_right = right_ord > 0
	if left_ord >= 0 and right_ord >= 0:
		print(a, b)
	return go_left, go_right

SternBrocotTreeProvider(1, 1, 1, 0, 387040).traversePreorder(logRational)

