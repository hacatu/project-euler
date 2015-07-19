#include <stdio.h>
#include <stdlib.h>
#include <primesieve.h>
#include <inttypes.h>

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

int main(void){
	primesieve_iterator pi;
	primesieve_init(&pi);
	uint64_t c = 0, s = 0, p;
	while(c < 40){
		p = primesieve_next_prime(&pi);
		if(powmod(10, 1000000000, 9*p) == 1){
			++c;
			s += p;
		}
	}
	primesieve_free_iterator(&pi);
	printf("%"PRIu64"\n", s);
}

