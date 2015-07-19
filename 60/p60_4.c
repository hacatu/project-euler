#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "prime sieves/sieve210f.h"

const uint64_t MAX_P10 = (uint64_t)((uint64_t)0-(uint64_t)1)/(uint64_t)10;

uint64_t cat(uint64_t a, uint64_t b){
	uint64_t t = 1;
	while(t < b){
		if(t > MAX_P10){
			perror("overflow");
			exit(3);
		}
		t *= 10;
	}
	return a*t + b; 
}

char isPrimeCat3(uint64_t b, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t *primes){
	return isPrime(cat(b, p1), primes) &&
		   isPrime(cat(b, p2), primes) &&
		   isPrime(cat(b, p3), primes);
}

char isPrimeCats4(uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t *primes){
	return isPrimeCat3(p1, p2, p3, p4, primes) &&
		   isPrimeCat3(p2, p1, p3, p4, primes) &&
		   isPrimeCat3(p3, p1, p2, p4, primes) &&
		   isPrimeCat3(p4, p1, p2, p3, primes);
}

int main(){
	FILE *file = fopen("prime sieves/primes1000000.data", "rb");
	if(!file){
		perror("Could not open prime data file \"prime sieves/primes1000000.data\"!");
		return 1;
	}
	uint64_t *primes = readPrimeFile(file);
	fclose(file);
	if(!primes){
		perror("Could not allocate memory for primes array!");
		return 2;
	}
	uint64_t p1, p2, p3, p4;
	for(uint64_t i1 = 1; i1 <= primes[0]; ++i1){
		p1 = primes[i1];
		printf("%llu\n", p1);
		for(uint64_t i2 = 1; i2 < i1; ++i2){
			p2 = primes[i2];
			for(uint64_t i3 = 1; i3 < i2; ++i3){
				p3 = primes[i3];
				for(uint64_t i4 = 1; i4 < i3; ++i4){
					p4 = primes[i4];
					if(isPrimeCats4(p1, p2, p3, p4, primes)){
						goto done;
					}
				}
			}
		}
	}
	done:
	printf("The primes %llu, %llu, %llu, and %llu add up to %llu\n", p1, p2, p3, p4, p1 + p2 + p3 + p4);
	free(primes);
}

