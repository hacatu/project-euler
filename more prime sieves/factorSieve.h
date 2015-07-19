#pragma once

#include <inttypes.h>

#ifndef MAX_FACTORS
//15 is the absolute maximum for a uint64_t
//9 is the absolute maximum for a uint32_t
#define MAX_FACTORS 9
#endif
#ifndef MAX
#define MAX 223092869
#endif

typedef struct {uint64_t factor, power;} PrimeFactor;

typedef PrimeFactor PrimeFactors[MAX_FACTORS];

uint64_t countDivisors(uint64_t n, PrimeFactors *fs);

uint64_t countDivisorsPow(uint64_t n, uint64_t p, PrimeFactors *fs);

PrimeFactors *factorSieve(uint64_t max);

void printFact(PrimeFactors fact);

