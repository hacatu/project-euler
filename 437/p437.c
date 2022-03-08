#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../util/factorization.h"

#define MAX_ODD_PRIMES 7

uint32_t (*sieve_factors(uint64_t max, uint32_t **_primes, size_t *_num_primes))[MAX_ODD_PRIMES]{
	uint64_t *is_composite = calloc(max/128 + 1, sizeof(uint64_t));
	if(!is_composite){
		return NULL;
	}
	uint32_t (*factors)[MAX_ODD_PRIMES] = calloc(max/2 + 1, sizeof(uint32_t[MAX_ODD_PRIMES]));
	if(!factors){
		free(is_composite);
		return NULL;
	}
	{
		uint64_t m;
		for(m = 3; m <= max/2; m += 3){
			factors[m][0] = 3;
		}
		for(; m <= max; m += 3){
			is_composite[(m - max/2)/64] |= 1ull << ((m - max/2)%64);
		}
	}
	for(uint64_t r = 0; r < 3; r += 2){
		for(uint64_t p = 5 + r; p <= max/2; p += 6){
			if(factors[p][0]){
				continue;
			}
			uint64_t m;
			for(m = p; m <= max/2; m += p){
				for(uint64_t i = 0; i < 7; ++i){
					if(!factors[m][i]){
						factors[m][i] = p;
						break;
					}
				}
			}
			for(; m <= max; m += p){
				is_composite[(m - max/2)/64] |= 1ull << ((m - max/2)%64);
			}
		}
	}
	uint32_t *primes = malloc((size_t)(1.25506*max/log(max))*sizeof(uint32_t));
	if(!primes){
		free(is_composite);
		free(factors);
		return NULL;
	}
	size_t num_primes = 0;
	for(uint64_t b = 11;; b += 10){
		for(uint64_t m = b; m < b + 10; m += 8){
			if(m > max){
				free(is_composite);
				void *tmp = realloc(primes, num_primes*sizeof(uint32_t));
				if(tmp){
					primes = tmp;
				}
				*_primes = primes;
				*_num_primes = num_primes;
				return factors;
			}
			if(m <= max/2 ? factors[m][0] == m : !(is_composite[(m - max/2)/64] & (1ull << ((m - max/2)%64)))){
				primes[num_primes++] = m;
			}
		}
	}
}

int is_primitive_root(uint64_t a, uint64_t p, const uint32_t factors[restrict static MAX_ODD_PRIMES]){
	if(powmod(a, (p-1)/2, p) == 1){
		return 0;
	}
	for(uint64_t i = 0; i < MAX_ODD_PRIMES; ++i){
		if(!factors[i]){
			return 1;
		}
		if(powmod(a, (p-1)/factors[i], p) == 1){
			return 0;
		}
	}
	return 1;
}

static const uint64_t max = 100000000;

int main(){
	uint32_t *primes;
	size_t num_primes;
	uint32_t (*factors)[MAX_ODD_PRIMES] = sieve_factors(max, &primes, &num_primes);
	if(!factors){
		printf("\e[1;31mERROR: Could not allocate primes and factors.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	printf("\e[1;32mFound %zu+1 candidate primes\e[0m\n", num_primes);
	uint64_t count = 1, sum = 5;
	#pragma omp parallel for reduction(+:count,sum)
	for(uint64_t i = 0; i < num_primes; ++i){
		uint64_t p = primes[i];
		uint64_t r = sqrt_mod(5, p);
		uint64_t a = (p+1)/2*(1 + r)%p;
		if(is_primitive_root(a, p, factors[(p-1)/2])){
			count += 1;
			sum += p;
			continue;
		}
		r = p - r;
		a = (p+1)/2*(1 + r)%p;
		if(is_primitive_root(a, p, factors[(p-1)/2])){
			count += 1;
			sum += p;
			continue;
		}
	}
	free(factors);
	free(primes);
	printf("\e[1;32mFound %"PRIu64" primes with Fibonacci primitive roots\nTheir sum is %"PRIu64"\e[0m\n", count, sum);
}

