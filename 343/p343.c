#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "factorization.h"

uint64_t largest_factor_sum(uint64_t max){
	uint64_t *n_factors = calloc(max + 2, sizeof(uint64_t));
	uint64_t *fn_factors = malloc((max + 2)*sizeof(uint64_t));
	fn_factors[2] = fn_factors[1] = 1;
	for(uint64_t n = 3; n <= max + 1; ++n){
		fn_factors[n] = n*(n - 3) + 3;
	}
	for(uint64_t n = 2; n <= max + 1; n <<= 1){
		n_factors[n] = 2;
	}
	for(uint64_t n = 6; n <= max + 1; n += 3){
		do{
			fn_factors[n] /= 3;
		}while(fn_factors[n]%3 == 0);
		if(fn_factors[n] == 1){
			fn_factors[n] = 3;
		}
	}
	for(uint64_t n = 3; n <= max + 1; n += 2){
		if(n_factors[n]){
			continue;
		}
		for(uint64_t m = n; m <= max + 1; m += n){
			n_factors[m] = n;
		}
		if(n%6 != 1){
			continue;
		}
		uint64_t r = sqrt_mod(n - 3, n);
		uint64_t rs[2] = {r, n - r};
		for(uint64_t i = 0; i < 2; ++i){
			r = rs[i];
			for(uint64_t m = (uint128_t)((n + 1) >> 1)*(3 + r)%n; m <= max + 1; m += n){
				do{
					fn_factors[m] /= n;
				}while(fn_factors[m]%n == 0);
				if(fn_factors[m] == 1){
					fn_factors[m] = n;
				}
			}
		}
	}
	uint64_t s = 0;
	for(uint64_t n = 2; n <= max + 1; ++n){
		s += (n_factors[n] > fn_factors[n] ? n_factors[n] : fn_factors[n]) - 1;
	}
	free(n_factors);
	free(fn_factors);
	return s;
}

int main(){
	uint64_t s = largest_factor_sum(2000000);
	printf("sum: %"PRIu64"\n", s);
}

