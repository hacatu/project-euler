#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <primesieve.h>

const uint64_t N = 20000000ull;
const uint64_t K = 5000000ull;

//const uint64_t N = 10ull;
//const uint64_t K = 3ull;

size_t primes_len;
uint64_t *primes;

uint64_t Pf(uint64_t n){
	uint64_t s = 0;
	for(uint64_t i = 0, p; i < primes_len && primes[i] <= n; ++i){
		p = primes[i];
		uint64_t t = 0;
		for(uint64_t q = n/p; q; q /= p){
			t += q;
		}
		s += t*p;
	}
	return s;
}

int main(){
	primes = primesieve_generate_primes(0, N, &primes_len, UINT64_PRIMES);
	uint64_t res = Pf(N) - Pf(K) - Pf(N - K);
	primesieve_free(primes);
	printf("%"PRIu64"\n", res);
}

