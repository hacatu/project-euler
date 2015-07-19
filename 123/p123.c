#include <stdio.h>
#include <inttypes.h>
#include <primesieve.h>

#define MIN 10000000000

uint64_t r(uint64_t pn, uint64_t n){
	//printf("%"PRIu64":%"PRIu64"\n", n, pn);
	if(!(n&1)){
		return 2;
	}
	return 2*pn*n%(pn*pn);
}

int main(void){
	primesieve_iterator p;
	primesieve_init(&p);
	for(uint64_t n = 1; ; ++n){
		if(r(primesieve_next_prime(&p), n) > MIN){
			printf("%"PRIu64"\n", n);
			break;
		}
	}
	primesieve_free(&p);
}

