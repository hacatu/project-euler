#include "factorization.h"
#include <sys/random.h>
#include <stdio.h>
#include <primesieve.h>

static inline int test_factor1(uint64_t n){
	int correct;
	if(is_prime_dmr(n)){
		correct = 1;
		for(uint64_t j = 0; j < 100; ++j){
			uint64_t x = rand_u64(0, n);
			uint64_t y = rand_u64(0, n);
			uint64_t a = rand_u64(0, n);
			//uint64_t p = factor1_lenstra(n, x, y, a, 10);
			uint64_t p = factor1_lenstra_montgomery(n, x, y, a, 10);
			//uint64_t p = factor1_pollard_rho_brent(n, x, 100);
			//uint64_t p = factor1_pollard_rho(n, x);
			if(p != 1 && p != n){
				printf("\e[1;31m%"PRIu64"|%"PRIu64" (\"factored\" a prime for x=%"PRIu64", y=%"PRIu64", a=%"PRIu64")\e[0m\n", p, n, x, y, a);
				//printf("\e[1;31m%"PRIu64"|%"PRIu64" (\"factored\" a prime for x=%"PRIu64")\e[0m\n", p, n, x);
				correct = 0;
				break;
			}
		}
		if(correct){
			//printf("\e[1;32m1|%"PRIu64" (a prime)\e[0m\n", n);
		}
	}else{
		correct = 0;
		for(uint64_t j = 0; j < 1000; ++j){
			uint64_t x = rand_u64(0, n);
			uint64_t y = rand_u64(0, n);
			uint64_t a = rand_u64(0, n);
			//uint64_t p = factor1_lenstra(n, x, y, a, 10);
			uint64_t p = factor1_lenstra_montgomery(n, x, y, a, 10);
			//uint64_t p = factor1_pollard_rho_brent(n, x, 100);
			//uint64_t p = factor1_pollard_rho(n, x);
			if(p != 1 && p != n){
				//printf("\e[1;32m%"PRIu64"|%"PRIu64" (factored a composite for x=%"PRIu64")\e[0m\n", p, n, x);
				correct = 1;
				break;
			}
		}
		if(!correct){
			printf("\e[1;33m(failed to factor a composite %"PRIu64")\e[0m\n", n);
		}
	}
	return correct;
}

const static factor_conf_t factor_conf = {
	.pollard_max=0,
	.pollard_stride=100,
	.lenstra_max=UINT64_MAX,
	.lenstra_bfac=10,
	.qsieve_max=0
};

static inline int test_factor(uint64_t n, uint64_t num_primes, uint64_t primes[static num_primes], factors_t *factors){
	factors->num_primes = 0;
	uint64_t d = factor_heuristic(n, num_primes, primes, &factor_conf, factors);
	if(d != 1 || factors_product(factors) != n){
		printf("\e[1;31mWhoops: factored %"PRIu64" wrong!\e[0m\n", n);
		return 0;
	}
	/*
	printf("\e[1;32m%"PRIu64" = ", n);
	for(uint64_t i = 0; i < factors->num_primes; ++i){
		printf("%"PRIu64, factors->factors[i].prime);
		if(factors->factors[i].power > 1){
			printf("^%"PRIu64, factors->factors[i].power);
		}
		printf(" ");
	}
	printf("\e[0m\n");
	*/
	return 1;
}

int main(){
	/*
	printf("\e[1;34mTesting lenstra-montgomery on 1000 random numbers\e[0m\n");
	for(uint64_t i = 0; i < 1000; ++i){
		uint64_t n = rand_u64(0, (1ull << 30) + 1);
		//printf("\e[1;34mTesting %"PRIu64"\e[0m\n", n);
		test_factor1(n);
	}
	*/
	size_t num_primes;
	uint64_t *primes = primesieve_generate_primes(1, 1ull << 16, &num_primes, UINT64_PRIMES);
	factors_t *factors = init_factors_t_ub(1ull << 30, num_primes, primes);
	printf("\e[1;34mTesting factorization on 4000000 random numbers\e[0m\n");
	for(uint64_t i = 0; i < 4000000; ++i){
		uint64_t n = rand_u64(0, 1ull << 30);
		test_factor(n, num_primes, primes, factors);
	}
	primesieve_free(primes);
	free(factors);
	/*
	uint64_t n = 1030530763;
	printf("\e[1;34mTesting lenstra-ecf with n=1030530763\e[0m\n");
	for(uint64_t i = 1;; ++i){
		uint64_t x = rand_u64(0, n);
		uint64_t y = rand_u64(0, n);
		uint64_t a = rand_u64(0, n);
		uint64_t p = factor1_lenstra(n, x, y, a);
		if(p != 1 && p != n){
			printf("Got factor %"PRIu64" in %"PRIu64" tries\n", p, i);
			break;
		}
	}
	*/
}

