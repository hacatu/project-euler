#pragma once

#include <inttypes.h>

typedef struct {uint64_t factor, power;} PrimeFactor;

uint64_t countDivisors(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m);

uint64_t countDivisorsPow(uint64_t m; uint64_t n, uint64_t p, const PrimeFactor fs[static n][m], uint64_t m);

uint64_t phi(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m);

int isPrimeFactorization(uint64_t m; uint64_t n, const PrimeFactor fs[static n][m], uint64_t m);

void *factorSieve(uint64_t max, uint64_t *max_factors);

void printFact(uint64_t m; const PrimeFactor fact[static m], uint64_t m);

