#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX 1000000
#define MIN 1000

#include <factorSieve.h>

int main(){
	PrimeFactors *fs = factorSieve(MAX);
	if(!fs){
		puts("malloc failed");
		return 1;
	}
	for(uint64_t n = 2, d; n <= MAX; ++n){
		if((countDivisorsPow(n, 2, fs) + 1)/2 > MIN){
			printf("%"PRIu64"\n", n);
			free(fs);
			return 0;
		}
	}
	puts("failed to find n");
	free(fs);
	return 1;
}

