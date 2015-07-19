#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define N 12

/* The real number of pairs is the total number of equal sized subsets with between 2 and n/2 elements.
 * Subsets with different numbers of elements cannot have the same sum so do not need to be compared.
 * Since the "set is strictly increasing", all elements are different and 1 element subsets do not need to be compared.
 * If a subset has more than n/2 elements, it is impossible to form one of the same size that is disjoint with it.
 * So the total number of pairs is sum((n i)*((n - i) i)/2!, i, 2, n/2) = (n!/2!)*sum(1/((i!)^2*(n - 2*i)!), i, 2, n/2).
 * But not all of these real pairs actually need to be checked.  For instance, for n = 4, the set is {a, b, c, d} and the
 * subset pairs are {a, b}&{c, d}, {a, c}&{b, d}, and {a, d}&{b, c}.  But a < b < c < d, so the first two can never be equal.
 * That is why only one set needs to be checked.  So if for the sorted subsets, the first element of the first is less than
 * the first element of the second and so on for all pairs of elements, they do not have to be compared.  The number of ways
 * this can happen for n and i is Ci*(n 2*i).  Ci is 2 for i = 2, 5 for i = 3, and 14 for i = 4.  Looking this up in Oeis, I
 * found Ci is the ith Catalan number, which has the formula Ci = (2*i i)/(i + 1).  Putting this all together, I got this:
 * sum(1/(2!)*(n i)*((n - i) i)-Ci*(n 2*i), i, 2, n/2)
 * =n!*sum((1/2 - 1/(i + 1))/((i!)^2*(n - 2*i)!), i, 2, n/2)
 * This can be rewritten in terms of the hypergeometric function:
 * 1/2*2F1((1-n)/2,-n/2;1;4)-2F1((1-n)/2,-n/2;2;4)+1/2
 */

uint64_t factorial[N + 1];

void _fillFactorial(void){
	for(uint64_t i = 0, p = 1; i <= N; p *= ++i){
		factorial[i] = p;
	}
}

uint64_t summand(uint64_t n, uint64_t i){
	return factorial[n]/factorial[n - 2*i]/factorial[i]/factorial[i]*(i - 1)/(2*i + 2);
}

int main(void){
	uint64_t s = 0;
	_fillFactorial();
	for(uint64_t i = 2; i <= N/2; ++i){
		s += summand(N, i);
	}
	printf("%"PRIu64"\n", s);
}

