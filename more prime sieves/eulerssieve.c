#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

uint64_t* sievePrimes(uint64_t max){
    char* is_composite = calloc(max + 1, sizeof(char));
    if(!is_composite){
		return 0;
	}
    uint64_t primes_s = max/log(max) + 1;
    uint64_t* primes = malloc(primes_s*sizeof(uint64_t));
    if(!primes){
		return 0;
	}
    uint64_t i = 1, p, p2, multiple;
    uint64_t* t;
    for(p = 2; p <= max; ++p){
        if(is_composite[p]){
			continue;
		}
        printf("%lu\n",p);
        primes[i++] = p;
        if(i == primes_s){
			primes_s *= 1.1;
			t = realloc(primes, primes_s*sizeof(uint64_t));
			if(!t){
				free(is_composite);
				return primes;
			}
			primes=t;
		}
        for(p2 = p; p2 <= max/p; ++p2){
            if(is_composite[p2]){
				continue;
			}
            multiple = p*p2;
            do{
                is_composite[multiple] = 1;
            }while((multiple *= p) <= max);
        }
    }
    free(is_composite);
    primes[0] = i;
    t = realloc(primes, i*sizeof(uint64_t));
    if(t){
		primes = t;
	}
    return primes;
}

//binary search primes for n
char is_prime(uint64_t n, uint64_t* primes){
	uint64_t max = primes[0];
	primes += 1;
	uint64_t min = 0, mid = max/2;
	while(n != primes[mid]){
		if(n > primes[mid]){
			min = mid + 1;
		}else{
			max = mid;
		}
		mid = max/2 + min/2;
		if(max == min){
			return 0;
		}
	}
	return 1;
}

int main(){
	uint64_t* P=sievePrimes(100);
	free(P);
	return 0;
}

