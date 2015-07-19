#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define MAX_BASE 174 //9*log(2^64)
#define MAX_EXPN 63
#define MIN(a, b) (((a)<(b))?(a):(b))

uint64_t digitSums[1000];

uint64_t powerSums[(MAX_BASE - 2 + 1)*(MAX_EXPN - 2 + 1)];
uint64_t powerSums_c = 0;

void init(void){
	for(uint64_t n = 0; n < 10; ++n){
		digitSums[n] = n;
	}
	for(uint64_t n = 10; n < 1000; ++n){
		digitSums[n] = digitSums[n%10] + digitSums[n/10];
	}
}

void addPowerSum(uint64_t n){
	powerSums[powerSums_c++] = n;
}

uint64_t digitSum(uint64_t n){
	uint64_t s = 0;
	while(n){
		s += digitSums[n%1000];
		n /= 1000;
	}
	return s;
}

int compare_uint64_t_p(uint64_t *a, uint64_t *b){
	if(*a < *b){
		return -1;
	}
	if(*a > *b){
		return 1;
	}
	return 0;
}

int main(void){
	init();
	for(uint64_t expn = 2; expn <= MAX_EXPN; ++expn){
		for(uint64_t base = 2, power; base <= MIN(MAX_BASE, (uint64_t)pow(UINT64_MAX, 1./(double)expn)); ++base){
			power = pow(base, expn);
			if(digitSum(power) == base){
				addPowerSum(power);
			}
		}
	}
	qsort(powerSums, powerSums_c, sizeof(uint64_t), (__compar_fn_t)compare_uint64_t_p);
	printf("%"PRIu64"\n", powerSums[29]);
}

