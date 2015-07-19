#include <stdio.h>
#include <stdlib.h>
#include <primesieve.h>
#include <inttypes.h>

#define MAX 100000

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

uint64_t A(uint64_t n){
	n *= 9;
	uint64_t k = 1;
	while(powmod(10, k, n) != 1){
		++k;
	}
	return k;
}

int main(void){
	uint64_t s = 10;//2 and 5 will never be divisible by any repunit, ever, and 3 will never be divisible by any R(10^n)
	size_t size;
	uint64_t *primes = primesieve_generate_primes(0, MAX, &size, UINT64_PRIMES);
	for(uint64_t i = 3, p, a; i < size; ++i){
		p = primes[i];
		a = A(p);//a must be of the form 2^a*5^b because p | R(10^n) iff A(p) | 10^n
		a >>= __builtin_ctz(a);
		while(a >= 5){
			if(a%5){
				a = 0;
				break;
			}
			a /= 5;
		}
		if(a != 1){
			s += p;
		}
	}
	primesieve_free(primes);
	printf("%"PRIu64"\n", s);
}

