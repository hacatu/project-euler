#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

/* Have b be half of the base instead.
 * b^2 + (2*b +- 1)^2 = L^2
 * b^2 + 4*b^2 +- 4*b + 1 = L^2
 * 5*b^2 +- 4*b + 1 = L^2
 * The reccurence relation for this equation is L(n) = 19*L(n-1) - 19*L(n-2) + L(n-3)
 */

#define MAX 12

int main(void){
	clock_t start = clock();
	uint64_t L1 = 5473, L2 = 305, L3 = 17, t, c = 3, s = 17 + 305 + 5473;
	while(c < MAX){
		t = 19*L1 - 19*L2 + L3;
		L3 = L2;
		L2 = L1;
		L1 = t;
		s += L1;
		++c;
	}
	printf("%"PRIu64" %fs\n", s, (double)(clock() - start)/CLOCKS_PER_SEC);
}

