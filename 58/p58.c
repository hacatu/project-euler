#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "prime sieves/sieve210.h"

int main(){
	uint64_t *P = sievePrimes(1000000000);
	uint64_t n = 1, c = 0, t = 1;
	for(uint64_t s = 2; n < 1000000000; ++s){
		for(int i = 0; i < 4; ++i){
			n += s;
			if(isPrime(n, P)){
				++c;
			}
		}
		t += 4;
		printf("%i/%i\n", c, t);
		if(10*c < t){
			printf("%i\n", s + 1);
			break;
		}
	}
	free(P);
}
