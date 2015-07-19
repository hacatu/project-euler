#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "factorSieve.h"

static void mark(PrimeFactors fact, uint64_t p, uint64_t m){
	uint64_t n = 0;
	while(fact[n].factor){//find the first empty spot to add the new prime factor
		++n;
	}
	fact[n].factor = p;
	while((m%p) == 0){//add 1 to the power on the prime p until m is not divisble by p
		++fact[n].power;
		m /= p;
	}
}

PrimeFactors *factorSieve(uint64_t max){
	PrimeFactors *facts = calloc(max + 1, sizeof(PrimeFactors));
	if(!facts){
		return NULL;
	}
	for(uint64_t p = 2; p <= max; ++p){
		if(facts[p][0].factor){//p is not prime
			continue;
		}//p is prime, mark off its multiples
		for(uint64_t m = p; m <= max; m += p){
			mark(facts[m], p, m);
		}
	}
	return facts;
}

uint64_t countDivisors(uint64_t n, PrimeFactors *fs){
	PrimeFactor *f = fs[n];
	uint64_t d = 1;
	for(uint64_t i = 0; i < MAX_FACTORS; ++i){
		d *= f[i].power + 1;
	}
	return d;
}

uint64_t countDivisorsPow(uint64_t n, uint64_t p, PrimeFactors *fs){
	PrimeFactor *f = fs[n];
	uint64_t d = 1;
	for(uint64_t i = 0; i < MAX_FACTORS; ++i){
		d *= f[i].power*p + 1;
	}
	return d;
}

void printFact(PrimeFactors fact){
	for(uint64_t i = 0; i < MAX_FACTORS && fact[i].factor; ++i){
		printf("%"PRIu64"^%"PRIu64" ", fact[i].factor, fact[i].power);
	}
	puts("");
}

