#include <stdio.h>
#include <primesieve.h>
#include <inttypes.h>
#include <stdlib.h>

uint64_t N = 5;

uint64_t binexp(uint64_t p, uint64_t n){
	uint64_t a = 1;
	for(;n; n >>= 1, p *= p){
		if(n&1){
			a *= p;
		}
	}
	return a;
}

int main(void){
	size_t num_primes;
	uint64_t *primes = primesieve_generate_primes(0, N + 1, &num_primes, UINT64_PRIMES);
	if(!primes){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate prime array!\e[0m\n");
		exit(EXIT_FAILURE);
	}
	uint64_t (*powers)[N + 1] = calloc(num_primes*(N + 1), sizeof(uint64_t));
	uint64_t (*terms)[N + 1] = calloc(num_primes*(N + 1), sizeof(uint64_t));
	if(!powers || !terms){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate power table!\e[0m\n");
		exit(EXIT_FAILURE);
	}
	for(size_t i = 0; i < num_primes; ++i){
		uint64_t p = primes[i];
		for(size_t n = 0; n <= N; ++n){
			for(size_t a = p; a <= n; a*= p){
				powers[i][n] += n/a;
			}
		}
	}
	for(size_t i = 0; i < num_primes; ++i){
		uint64_t p = primes[i];
		uint64_t kpowers = 0;
		for(size_t n = 0; n <= N; ++n){
			uint64_t npower = powers[i][n];
			uint64_t a = (n - 1)*npower + 1 - 2*kpowers;
			kpowers += npower;
			terms[i][n] = (binexp(p, a) - 1)/(p - 1);
		}
	}
	free(powers);
	primesieve_free(primes);
	uint64_t s = 0;
	for(size_t n = 0; n <= N; ++n){
		uint64_t p = 1;
		for(size_t i = 0; i < num_primes; ++i){
			p *= terms[i][n];
		}
		s += p;
	}
	free(terms);
	printf("S(%"PRIu64")=%"PRIu64"\n", N, s);
}

