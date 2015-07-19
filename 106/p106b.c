#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_sf_hyperg.h>
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

double checksNeeded(double n){
	return 0.5*gsl_sf_hyperg_2F1((1-n)/2,-n/2,1,4)-gsl_sf_hyperg_2F1((1-n)/2,-n/2,2,4)+0.5;
}

int main(void){
	printf("%f\n", checksNeeded(4));
}

