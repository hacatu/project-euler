#include <stdio.h>
#include <inttypes.h>
#include <math.h>

/* Have b be half of the base instead.
 * b^2 + (2*b +- 1)^2 = L^2
 * b^2 + 4*b^2 +- 4*b + 1 = L^2
 * 5*b^2 +- 4*b + 1
 */

#define MAX 12

int main(void){
	uint64_t s = 0;
	for(uint64_t c = 0, b = 1, l2, l;; ++b){
		l2 = 5*b*b - 4*b + 1;
		l = sqrtl(l2);
		if(l2 == l*l){
			printf("%"PRIu64"\n", l);
			s += l;
			++c;
			if(c == MAX){
				break;
			}
		}
		l2 += 8*b;
		l = sqrt(l2);
		if(l2 == l*l){
			printf("%"PRIu64"\n", l);
			s += l;
			++c;
			if(c == MAX){
				break;
			}
		}
	}
	printf("%"PRIu64"\n", s);
}

