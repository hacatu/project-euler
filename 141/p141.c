/* n/d = q
 * n%d = r
 * d, q, and r are consecutive terms in a geometric sequence
 * n, d, q, and r are positive integers.
 * 
 * Iterating over a and b where a is the firt term and b is the common ratio (note this neglects non-integer b):
 * n = d*q + r
 * There are these forms:
 * 1: n = (a)*(a*b) + (a*b^2) //r > d
 * 2: n = (a)*(a*b^2) + (a*b) //r > d
 * 3: n = (a*b)*(a) + (a*b^2) //r > d
 * 4: n = (a*b)*(a*b^2) + (a)
 * 5: n = (a*b^2)*(a) + (a*b)
 * 6: n = (a*b^2)*(a*b) + (a) //same as 4
 * 
 * n = a*(a*b^3 + 1)
 * n = a*b*(a*b + 1)
 */
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <hac_avl.h>

#define MAX 100000

int compUint64_ts(uint64_t a, uint64_t b){
	if(a > b){
		return -1;
	}else if(a < b){
		return 1;
	}
	return 0;
}

int main(void){
	HAC_AVL_T_NEW(uint64_t);
	HAC_AVL_T(uint64_t) psquares = HAC_AVL_EMPTY(uint64_t);
	uint64_t s = 0;
	for(uint64_t a = 1; a*(a*8 + 1) < MAX; ++a){
		for(uint64_t b = 2, n, r;; ++b){
			n = a*(a*b*b*b + 1);
			if(n >= MAX){
				break;
			}
			r = sqrtl(n);
			if(n == r*r){
				printf("%"PRIu64"\n", n);
				if(HAC_AVL_INSERT(uint64_t, &psquares, n, compUint64_ts(e1, e2))){
					s += n;
				}
			}
		}
	}
	for(uint64_t a = 1; a*2*(a*2 + 1) < MAX; ++a){
		for(uint64_t b = 2, n, r;; ++b){
			n = a*b*(a*b + 1);
			if(n >= MAX){
				break;
			}
			r = sqrtl(n);
			if(n == r*r){
				printf("%"PRIu64"\n", n);
				if(HAC_AVL_INSERT(uint64_t, &psquares, n, compUint64_ts(e1, e2))){
					s += n;
				}
			}
		}
	}
	HAC_AVL_DELETE(uint64_t, &psquares);
	printf("%"PRIu64"\n", s);
}

