#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#define MAX 100000000

/* without complex divisors:
 * sum(s(n), n, 1, m)
 * rewite in terms of sieve:
 * n + n*(n + 1)/2 - 1 + sum(d + sum(d + k, k, d + 1, n/d), d, 2, sqrt(n)) - 1
 * let r = floor(sqrt(n)) and b = floor(n/d) and expand
 * (n*(n + 3) - r^2*(r + 1))/2 + sum(d*b + b*(b + 1)/2, d, 2, r)
 */
uint64_t s3(uint64_t n){
	uint64_t r = sqrtl(n);
	uint64_t s = (n*(n + 3) - r*r*(r + 1))/2;
	for(uint64_t d = 2, b; d <= r; ++d){
		b = n/d;
		s += d*b + b*(b + 1)/2;
	}
	return s;
}

/* All complex numbers can be written as k*(a +- b*i) with a and b coprime
 * x divisible by (k*(a - b*i))
 * x*(a + b*i) divisible by (k*(a^2 + b^2)
 * x divisible by k*(a^2 + b^2)
 * So if n is the upper bound then for all pairs a and b if a != b
 * there are n/(a^2 + b^2) multiples of (a +- b*i) and of (b +- a*i) less than n.
 * and there are n/(k*(a^2 + b^2)) multiples of k*(a +- b*i) and of k*(b +- a*i).
 * Let c = a^2 + b^2 and sum n/(k*c)*2*k*(a + b) over k
 * The only pair where a == b is 1 +- 1*i because a and b must be coprime.
 * This works just like a +- b*i except b +- a*i is the same so
 * sum n/(2*k)*2*k
 * This can be calculated using the s function we already wrote.
 * To actually get the coprime numbers, use the farey sequence of order sqrt(n) - 1
 */
uint64_t g3(uint64_t n){
	uint64_t r = sqrtl(n);
	uint64_t s = 0;
	s += 2*s3(n/2);
	for(uint64_t a1 = 0, b1 = 1, a2 = 1, b2 = r, k, t; a2 < b2;){
		uint64_t c = a2*a2 + b2*b2;
		s += 2*(a2 + b2)*s3(n/c);
		k = (r + b1)/b2;
		t = a2;
		a2 = k*a2 - a1;
		a1 = t;
		t = b2;
		b2 = k*b2 - b1;
		b1 = t;
	}
	return s;
}

int main(void){
	printf("%"PRIu64"\n", s3(MAX) + g3(MAX));
}

