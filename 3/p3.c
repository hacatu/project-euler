#include <stdio.h>
#include <math.h>
#include <primesieve.h>
#include <inttypes.h>
#include <time.h>

#define NUMBER 600851475143
#define _STR(a) #a
#define STR(a) _STR(a)

int main(void){
	clock_t start = clock();
	uint64_t r = sqrt(NUMBER);
	size_t n;
	uint64_t *ps = primesieve_generate_primes(0, r, &n, UINT64_PRIMES);
	uint64_t m = NUMBER;
	for(uint64_t i = 0, p; i < n; ++i){
		p = ps[i];
		if(p > r){
			break;
		}
		if(!(m%p)){
			m /= p;
			r = sqrt(m);
		}
	}
	printf("The largest prime factor of "STR(NUMBER)" is %"PRIu64".\nCalculated in %f seconds.\n", m, (double)(clock() - start)/CLOCKS_PER_SEC);
}

