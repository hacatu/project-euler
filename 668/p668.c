#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <primesieve.h>

static uint64_t M = 10000000000ull;
//#define M 100ull

int main(){
	uint64_t count = 0;
	size_t size;
	uint64_t *primes = primesieve_generate_primes(1, M, &size, UINT64_PRIMES);
	#pragma omp parallel for reduction(+:count)
	for(size_t i = 0; i < size; ++i){
		count += primes[i] <= M/primes[i] ? primes[i] : M/primes[i];
	}
	primesieve_free(primes);
	printf("count: %"PRIu64"\n", M-count);
}

