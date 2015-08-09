/* n/d = q
 * n%d = r
 * d, q, and r are consecutive terms in a geometric sequence
 * n, d, q, and r are positive integers.
 * 
 * Iterating over a and b where a is the firt term and b is the second term:
 * n = d*q + r
 * a, b, b^2/a
 * There are these forms:
 * 1: n = (a)*(b) + (b^2/a) //r > d
 * 2: n = (a)*(b^2/a) + (b) //r > d
 * 3: n = (b)*(a) + (b^2/a) //r > d
 * 4: n = (b)*(b^2/a) + (a)
 * 5: n = (b^2/a)*(a) + (b)
 * 6: n = (b^2/a)*(b) + (a) //same as 4
 * 
 * n = b^3/a + a
 * n = b*(b + 1)
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define MAX 1000000000000

int compUint64_ts(uint64_t a, uint64_t b){
	if(a > b){
		return -1;
	}else if(a < b){
		return 1;
	}
	return 0;
}

void mark(uint64_t *fsms, uint64_t p, uint64_t m){
	uint64_t n = m, r = 0;
	while(!(n%p)){//add 1 to the power on the prime p until m is not divisble by p
		n /= p;
		++r;
	}
	r = (r + 1)/2;
	while(r){
		fsms[m] *= p;
		--r;
	}
}

int main(void){
	uint64_t max = sqrtl(MAX);
	uint64_t *fsms = malloc(max*sizeof(uint64_t));
	for(uint64_t i = 0; i < max; ++i){
		fsms[i] = 1;
	}
	for(uint64_t p = 2; p < max; ++p){
		if(fsms[p] != 1){//p is not prime
			continue;
		}//p is prime, mark off its multiples
		for(uint64_t m = p; m < max; m += p){
			mark(fsms, p, m);
		}
	}
	uint64_t s = 0;
	//puts("first loop");
	for(uint64_t a = 1; a < max; ++a){
		for(uint64_t b = (a/fsms[a] + 1)*fsms[a], n, r;; b += fsms[a]){
			n = b*b*b/a + a;
			if(n >= MAX){
				break;
			}
			r = sqrtl(n);
			if(n == r*r){
				//printf("%"PRIu64"\n", n);
				s += n;
			}
		}
	}
	//puts("second loop");
	//Does not find any numbers...
	/*
	for(uint64_t a = 1; a < max; ++a){
		for(uint64_t b = (a/fsms[a] + 1)*fsms[a], n, r;; b += fsms[a]){
			n = b*(b + 1);
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
	*/
	free(fsms);
	printf("%"PRIu64"\n", s);
}

