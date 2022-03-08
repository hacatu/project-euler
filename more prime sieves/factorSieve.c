#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "factorSieve.h"

static void mark(PrimeFactor fact[], uint64_t p, uint64_t m){
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

void *factorSieve(uint64_t max, uint64_t *max_factors){
	{
		static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
		uint64_t i = 0;
		for(uint64_t p = 1; i < 15 && p <= max; p *= small_primes[i++]);
		*max_factors = i;
	}
	PrimeFactor (*facts)[*max_factors] = calloc((max + 1)* *max_factors, sizeof(PrimeFactor));
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

uint64_t countDivisors(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m){
	const PrimeFactor *f = fs[n];
	uint64_t d = 1;
	for(uint64_t i = 0; i < m; ++i){
		d *= f[i].power + 1;
	}
	return d;
}

uint64_t countDivisorsPow(uint64_t m; uint64_t n, uint64_t p, const PrimeFactor fs[static n][m], uint64_t m){
	const PrimeFactor *f = fs[n];
	uint64_t d = 1;
	for(uint64_t i = 0; i < m; ++i){
		d *= f[i].power*p + 1;
	}
	return d;
}

uint64_t phi(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m){
	const PrimeFactor *f = fs[n];
	uint64_t r = 1;
	for(uint64_t i = 0; i < m && f[i].factor; ++i){
		r *= f[i].factor - 1;
		for(uint64_t j = 1; j < f[i].power; ++j){
			r *= f[i].factor;
		}
	}
	return r;
}

int isPrimeFactorization(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m){
	const PrimeFactor *f = fs[n];
	return f[0].power == 1 && !f[1].power;
}

void printFact(uint64_t m; const PrimeFactor fact[static m], uint64_t m){
	for(uint64_t i = 0; i < m && fact[i].factor; ++i){
		printf("%"PRIu64"^%"PRIu64" ", fact[i].factor, fact[i].power);
	}
	puts("");
}

