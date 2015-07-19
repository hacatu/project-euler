#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "prime sieves/sieve210.h"

#define max(a,b) a>b?a:b

const int MAX_P = 2;
const int MIN_P = 56003;

char primesInFamily(uint64_t p, const uint64_t *P, uint64_t mask, char digit){
	//puts("pif: called");
	char c = 1;
	for(char multiple = 1; multiple < 10 - digit; ++multiple){
		p += mask;
		//printf("pif: p:%li\n", p);
		if(isPrime(p, P)){
			//puts("p is prime");
			++c;
		}
	}
	//printf("pif: return:%i\n", (int)c);
	return c;
}

char primesInGroup(uint64_t p, const uint64_t *P, uint64_t mask, uint64_t power, char digit){
	//puts("pig: called");
	uint64_t cmask = mask/power;
	while(cmask != 1){
		if(cmask%10){
			cmask = mask - power;
			//printf("pig: m/cm = %li/%li\n", mask, cmask);
			return max(primesInGroup(p, P, mask, 10*power, digit),
					   primesInGroup(p, P,cmask, 10*power, digit));
		}
		cmask /= 10;
		power *= 10;
	}
	return primesInFamily(p, P, mask, digit);
}

int main(){
	uint64_t *P = sievePrimes(2000000);
	uint64_t P_size = *P;
	uint64_t mask, power, p, n;
	uint64_t *primes = P + 1;
	for(uint64_t i = 0; i < P_size; ++i){
		p = primes[i];
		if(p < MIN_P){
			continue;
		}
		//printf("p:%li\n", p);
		for(char digit = 0; digit <= MAX_P; ++digit){
			//printf("  digit:%i\n", digit);
			power = 10;
			mask = 0;//mask's one's digit should be 0
			n = p/10;
			while(n){
				//printf("    n:%li\n", n);
				if(n%10 == digit){
					mask += power;
				}
				power *= 10;
				n /= 10;
			}
			if(mask == 0){
				continue;
			}
			//printf("  mask:%li\n", mask);
			if(primesInGroup(p, P, mask, 10, digit) >= 10 - MAX_P){
				printf("found solution: %li\n", p);
				return 0;
			}
		}
	}
	free(P);
}

