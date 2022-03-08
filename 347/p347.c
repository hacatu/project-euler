#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <primesieve.h>

#define MAX 10000000

int main(void){
	size_t ps_len;
	uint64_t *ps = primesieve_generate_primes(0, (MAX >> 1) + 1, &ps_len, UINT64_PRIMES);
	if(!ps){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory!\e[0m");
		exit(1);
	}
	uint64_t s = 0;
	#pragma omp parallel for reduction(+:s)
	for(size_t j = 1; j < ps_len; ++j){
		uint64_t q = ps[j];
		for(size_t i = 0; i < j; ++i){
			uint64_t p = ps[i], m = p*q;
			if(m > MAX){
				break;
			}
			uint64_t M = m;
			for(uint64_t b = m; b <= MAX; b *= p){
				uint64_t m = b;
				for(uint64_t n = m*q; n <= MAX; m = n, n *= q);
				if(m > M){
					M = m;
				}
			}
			s += M;
		}
	}
	printf("%"PRIu64"\n", s);
}

