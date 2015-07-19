#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "prime sieves/sieve210f.h"

const uint64_t MAX_P10 = (uint64_t)((uint64_t)0-(uint64_t)1)/(uint64_t)10;

inline uint64_t cat(uint64_t a, uint64_t b){
	uint64_t t = 1;
	while(t <= b){
		if(t > MAX_P10){
			perror("overflow");
			exit(3);
		}
		t *= 10;
	}
	return a*t + b; 
}

int main(){
	FILE *file = fopen("prime sieves/primes100000000.data", "rb");
	if(!file){
		perror("Could not open prime data file \"prime sieves/primes100000000.data\"!");
		return 1;
	}
	uint64_t *primes = readPrimeFile(file);
	fclose(file);
	if(!primes){
		perror("Could not allocate memory for primes array!");
		return 2;
	}
	char *isPrimeCatA = calloc(3245*3245, sizeof(char));
	if(!isPrimeCatA){
		perror("Could not allocate memory for cats array!");
		free(primes);
		return 2;
	}
	uint64_t i1, i2, i3, i4, i5, p1, p2, p3, p4, p5;
	for(i1 = 2; i1 < 3245; ++i1){
		p1 = primes[i1];
		for(i2 = 2; i2 < i1; ++i2){
			p2 = primes[i2];
			isPrimeCatA[i1*3245 + i2] = isPrime(cat(p1, p2), primes) && isPrime(cat(p2, p1), primes);
		}
	}
	for(i1 = 2; i1 < 3245; ++i1){
		for(i2 = 2; i2 < i1; ++i2){
			if(!isPrimeCatA[i1*3245 + i2]){
				continue;
			}
			for(i3 = 2; i3 < i2; ++i3){
				if(!isPrimeCatA[i1*3245 + i3] ||
				   !isPrimeCatA[i2*3245 + i3]){
					   continue;
				   }
				for(i4 = 2; i4 < i3; ++i4){
					if(!isPrimeCatA[i1*3245 + i4] ||
					   !isPrimeCatA[i2*3245 + i4] ||
					   !isPrimeCatA[i3*3245 + i4]){
						   continue;
					   }
					for(i5 = 2; i5 < i4; ++i5){
						if(isPrimeCatA[i1*3245 + i5] &&
						   isPrimeCatA[i2*3245 + i5] &&
						   isPrimeCatA[i3*3245 + i5] &&
						   isPrimeCatA[i4*3245 + i5]){
							goto done;
						}
					}
				}
			}
		}
	}
	done:
	p1 = primes[i1];
	p2 = primes[i2];
	p3 = primes[i3];
	p4 = primes[i4];
	p5 = primes[i5];
	printf("The primes %llu, %llu, %llu, %llu, and %llu add up to %llu\n", p1, p2, p3, p4, p5, p1 + p2 + p3 + p4 + p5);
	free(primes);
	free(isPrimeCatA);
}

