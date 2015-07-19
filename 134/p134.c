#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <primesieve.h>

#define MAX 1000000

uint64_t modinv(uint64_t a, uint64_t m){
	int64_t s1 = 0, s0 = 1;
	uint64_t r1 = m, r0 = a, q, t;
	while(r1){
		q = r0/r1;
		t = r0;
		r0 = r1;
		r1 = t - q*r1;
		t = s0;
		s0 = s1;
		s1 = t - q*s1;
	}
	return s0 < 0 ? s0 + m : s0;
}

uint64_t moddiv(uint64_t a, uint64_t b, uint64_t m){
	uint64_t i = modinv(b, m);
	return a*i%m;
}

uint64_t l10(uint64_t n){
	uint64_t t = 10;
	while(t < n){
		t *= 10;
	}
	return t;
}

int main(void){
	size_t size;
	uint64_t *primes = primesieve_generate_primes(5, MAX + 100, &size, UINT64_PRIMES);
	if(!primes){
		puts("Could not allocate primes.");
		return 1;
	}
	uint64_t s = 0;
	for(uint64_t i = 0, p1, p2, t;; ++i){
		p1 = primes[i];
		if(p1 > MAX){
			break;
		}
		p2 = primes[i + 1];
		t = l10(p1);
		s += p2*moddiv(p1, p2, t);
	}
	primesieve_free(primes);
	printf("%"PRIu64"\n", s);
}

