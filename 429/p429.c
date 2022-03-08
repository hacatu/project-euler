#define _POSIX_C_SOURCE 201805L
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <primesieve.h>
#include <time.h>

/* d is a unitary divisor of n means d|n and gcd(d, n/d) = 1
 * find the sum of the squares of the unitary divisors of 1e8! mod 1e9+9
 * We need to split the factors of 1e8!, a unitary divisor must have either all
 * or none of the powers of a prime.  1e8! is divisible by every prime up to 1e8.
 * There are a lot of those though.  We need the sum of not just the highest prime powers,
 * but the products of all combinations of them.  There are 5761455 such primes, so there
 * are 2**5761455 combinations of them, which is far too many.
 */

uint64_t powmod(unsigned __int128 b, uint64_t e, uint64_t n){
	unsigned __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return (uint64_t)r;
}

int main(){
	size_t primes_len;
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t *primes = primesieve_generate_primes(0ull, 100000000ull, &primes_len, UINT64_PRIMES);
	uint64_t sum = 1;
	for(uint64_t i = 0; i < primes_len; ++i){
		uint64_t power = 0;
		uint64_t prime = primes[i];
		for(uint64_t prime_power = prime; prime_power <= 100000000ull ; prime_power *= prime){
			power += 100000000ull/prime_power;
		}
		power <<= 1;
		sum = sum*(1 + powmod(prime, power, 1000000009ull)) % 1000000009ull;
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	primesieve_free(primes);
	printf("%"PRIu64"\n%fs\n", sum,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
}

