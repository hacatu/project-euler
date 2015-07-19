#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "prime sieves/sieve210f.h"

int main(){
	FILE *primes = fopen("prime sieves/primes1000000000.data","r");
	if(!primes){
		puts("no prime file.");
		return 0;
	}
	//printFile(primes);
	uint64_t n = 1, c = 0, t = 1;
	for(uint64_t s = 2; n < 1000000000; s += 2){
		for(int i = 0; i < 3; ++i){
			n += s;
			if(isPrimeFile(n, primes)){
				++c;
			}
		}
		n += s;
		t += 4;
		//printf("%llu/%llu\n", c, t);
		if(10*c < t){
			printf("%llu\n", s + 1);
			break;
		}
	}
	fclose(primes);
}
