#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

uint64_t* sievePrimes(uint64_t max){
    char* is_composite = calloc(max + 1, sizeof(char));
    if(!is_composite){
		return 0;
	}
    uint64_t primes_s = max/log(max) + 2;
    uint64_t* primes = malloc(primes_s*sizeof(uint64_t));
    if(!primes){
		return 0;
	}
    uint64_t i = 1, multiple, p;
    uint64_t* t;
    for(p = 2; p<=max; ++p){
		if(is_composite[p]){
			continue;
		}
		//printf("%lu\n",p);
		primes[i++] = p;
		if(i == primes_s){
			primes_s = i*1.1;
			t = realloc(primes, primes_s*sizeof(uint64_t));
			if(!t){
				return primes;
			}
			primes = t;
		}
		for(multiple = p+p; multiple <= max; multiple += p){
			is_composite[multiple]=1;
		}
	}
	primes[0] = i;
	t = realloc(primes, i*sizeof(uint64_t));
	primes = t;
	free(is_composite);
    return primes;
}

int main(){
	uint64_t* P = sievePrimes(1000000);
	free(P);
	return 0;
}

