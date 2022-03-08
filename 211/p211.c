#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define M 64000000
#define M 640000

/* \sigma_x(n) = \prod_{i=1}^r\frac{p_i^{x(a_i+1)}-1}{p_i^x-1}
 * \sigma_2(n) = \prod_{i=1}^r\frac{p_i^{2(a_i+1)}-1}{p_i^2-1}
 */

uint64_t *sigma_x_sieve(uint64_t max, uint64_t x){
	uint64_t *buf = malloc(max*sizeof(uint64_t));
	if(!buf){
		return NULL;
	}
	for(uint64_t i = 1; i < max; ++i){
		buf[i] = 1;
	}
	for(uint64_t p = 2; p < max; ++p){
		if(buf[p] != 1){
			continue;
		}
		uint64_t px = 1;
		for(uint64_t i = 0; i < x; ++i){
			px *= p;
		}
		buf[p] = px + 1;
		for(uint64_t m = 2*p; m < max; m += p){
			uint64_t pxa = px;
			for(uint64_t d = m; !(d%p); d /= p){
				pxa *= px;
			}
			buf[m] *= (pxa - 1)/(px - 1);
		}
	}
	return buf;
}

int main(int argc, char **argv){
	uint64_t *sigma_xs = sigma_x_sieve(M, 2);
	uint64_t s = 0;
	for(uint64_t n = 1; n < M; ++n){
		uint64_t r = sqrt(sigma_xs[n]);
		if(r*r == sigma_xs[n]){
			//printf("%"PRIu64"\n", n);
			s += n;
		}
	}
	free(sigma_xs);
	printf("%"PRIu64"\n", s);
}

