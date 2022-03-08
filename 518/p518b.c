#define _POSIX_C_SOURCE 201805L
#define MAX 100000000ULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include "../more prime sieves/factorSieve.h"

#define DMR_PRIMES (uint64_t[]){2, 7, 61}
#define DMR_PRIMES_C 3

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

const uint64_t offset[30] = {
	0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3,
	3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 6, 7,
	7, 7, 7, 7, 7, 8,};

const uint64_t step[8] = {7-1, 11-7, 13-11, 17-13, 19-17, 23-19, 29-23, 2};

int isPrimeBitarray(uint64_t n, const uint64_t is_composite[static n/240+1]){
	if(!(n%2 && n%3 && n%5) || n < 7){
		return n == 2 || n == 3 || n == 5;
	}
	uint64_t index = ((n/30)<<3) + offset[n%30];
	return !(is_composite[index>>6] & (1ull << (index&63)));
}

uint64_t *mark_primes(uint64_t max){
	uint64_t *is_composite = calloc(max/240+1, sizeof(uint64_t));
	if(!is_composite){
		return NULL;
	}
	is_composite[0] = 1;
	for(uint64_t n = 7, i = 1; n*n <= max; n += step[i++&7]){
		if(is_composite[i>>6] & (1ull << (i&63))){
			continue;
		}
		for(uint64_t m = n*n, j = i; m <= max; m += n*step[j++&7]){
			uint64_t k = ((m/30)<<3) + offset[m%30];//we need a not stupid way to calculate k
			is_composite[k>>6] |= 1ull << (k&63);
		}
	}
	return is_composite;
}

int main(void){
	//a = k*x**2 - 1, b = k*x*y - 1, c = k*y**2 - 1 where y > x coprime
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t *is_composite = mark_primes(MAX);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf("Got primes in %fs\n",
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
	uint64_t s = 0, y_max = sqrt(MAX >> 1);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	#pragma omp parallel for reduction(+:s)
	for(uint64_t y = 2; y <= y_max; ++y){
		uint64_t y2 = y*y;
		for(uint64_t k = 2, c = (y2 << 1) - 1; c < MAX; ++k, c += y2){
			if(!isPrimeBitarray(c, is_composite)){
				continue;
			}
			for(uint64_t x = 1; x < y; ++x){
				uint64_t a = k*x*x - 1, b = k*x*y - 1;
				if(isPrimeBitarray(b, is_composite) && isPrimeBitarray(a, is_composite) && gcd(y, x) == 1){
					//printf("(%"PRIu64", %"PRIu64", %"PRIu64")\n", a, b, c);
					s += a + b + c;
				}
			}
		}
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	free(is_composite);
	printf("%"PRIu64"\n%fs\n", s,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
}

