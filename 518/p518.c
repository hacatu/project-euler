#define _POSIX_C_SOURCE 201805L
#define MAX 100000000ULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include "../more prime sieves/factorSieve.h"

#define DMR_PRIMES (uint64_t[]){2, 13, 23, 1662803}
#define DMR_PRIMES_C 4

uint64_t gcd(uint64_t a, uint64_t b){
	if(a < b){
		b %= a;
	}
	while(1){
		if(!b){
			return a;
		}
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
	}
}

uint64_t powmod(unsigned __int128 b, uint64_t e, uint64_t n){
	unsigned __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return (uint64_t)r;
}

int isPrimeDMR(uint64_t n){
	uint64_t s, d;//s, d | 2^s*d = n - 1
	if(n%2 == 0){
		return n == 2;
	}
	--n;
	s = __builtin_ctz(n);
	d = n>>s;
	++n;
	for(uint64_t i = 0, a, x; i < DMR_PRIMES_C; ++i){
		a = DMR_PRIMES[i];
		if(a >= n){
			break;
		}
		x = powmod(a, d, n);
		if(x == 1 || x == n - 1){
			goto CONTINUE_WITNESSLOOP;
		}
		for(a = 0; a < s - 1; ++a){
			x = powmod(x, 2, n);
			if(x == 1){
				return 0;
			}
			if(x == n - 1){
				goto CONTINUE_WITNESSLOOP;
			}
		}
		return 0;
		CONTINUE_WITNESSLOOP:;
	}
	return 1;
}

int main(void){
	struct timespec start_time, end_time;
	uint64_t s = 0;
	/*for(uint64_t p = 2; p*p <= MAX; p += 1){
		for(uint64_t q = 1; q < p; q += 1){
			if(gcd(p, q) != 1){
				continue;
			}
			for(uint64_t a = q == 1 ? 3 : 1; a*p*p <= MAX; a += 1){
				if(isPrime(a*q*q - 1) && isPrime(a*p*q - 1) && isPrime(a*p*p - 1)){
					printf("(%"PRIu64", %"PRIu64", %"PRIu64")\n", a*q*q - 1, a*q*p - 1, a*p*p - 1);
					s += a*((p + q)*(p + q) - p*q) - 3;
				}
			}
		}
	}*/
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t max_factors;
	void *_factorss = factorSieve(MAX, &max_factors);
	PrimeFactor (*factorss)[max_factors] = _factorss;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	if(!factorss){
		fprintf(stderr, "\e[1;31mERROR: Could not generate factorizations (%fGB).\e[0m\n", max_factors*MAX*sizeof(uint64_t)*0x1p-30);
		exit(1);
	}
	printf("Generated factorizations in %fs\n",
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	for(uint64_t i = 2; i <= MAX - 2; ++i){
		if(!isPrimeFactorization(i, factorss, max_factors)){
			continue;
		}
		//printf("a = %"PRIu64"\n", i);
		PrimeFactor *p_1_factors = factorss[i + 1];
		PrimeFactor q_factors[max_factors];
		memset(q_factors, 0, max_factors*sizeof(PrimeFactor));
		for(uint64_t i = 0; i < max_factors && p_1_factors[i].factor; ++i){
			q_factors[i].factor = p_1_factors[i].factor;
		}
		uint64_t q = 1;
		while(1){
			//printf(" q = %"PRIu64"\n", q);
			for(uint64_t p = q + 1; p <= sqrt(MAX*q*q/(i + 1)); ++p){
				if(gcd(p, q) != 1){
					continue;
				}
				uint64_t b = (i + 1)/q*p - 1, c = (b + 1)/q*p - 1;
				//printf("  p = %"PRIu64"\n  b = %"PRIu64"\n  c = %"PRIu64"\n", p, b, c);
				if(isPrimeFactorization(b, factorss, max_factors) &&
					isPrimeFactorization(c, factorss, max_factors)){
					//printf("(%"PRIu64", %"PRIu64", %"PRIu64")\n", i, b, c);
					s += i + b + c;
				}
			}
			//process
			uint64_t i = 0;
			for(; i < max_factors; ++i){
				if(q_factors[i].power < (p_1_factors[i].power >> 1)){
					++q_factors[i].power;
					for(uint64_t j = 0; j < i; ++j){
						q_factors[j].power = 0;
					}
					q = 1;
					for(uint64_t j = i; j < max_factors && p_1_factors[j].factor; ++j){
						for(uint64_t i = 0; i < q_factors[j].power; ++i){
							q *= q_factors[j].factor;
						}
					}
					break;
				}
			}
			if(i == max_factors){
				break;
			}
		}
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	free(factorss);
	printf("%"PRIu64"\n%fs\n", s,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
}

