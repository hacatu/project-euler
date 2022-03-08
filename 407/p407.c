#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "../more prime sieves/factorSieve.h"

uint64_t make_idempotent(uint64_t n, int64_t a){
	int64_t b = n/a;
	int64_t
		s_0 = 0, s_1 = 1,
		t_0 = 1, t_1 = 0,
		r_0 = b, r_1 = a;
	while(r_0){
		int64_t q = r_1/r_0;
		int64_t t = r_0;
		r_0 = r_1%r_0;
		r_1 = t;
		t = s_0;
		s_0 = s_1 - q*s_0;
		s_1 = t;
		t = t_0;
		t_0 = t_1 - q*t_0;
		t_1 = t;
	}
	return s_1 < 0 ? b*t_1 : a*s_1;
}

uint64_t find_largest_idempotent(uint64_t n, PrimeFactor factors[static MAX_FACTORS]){
	uint64_t mask_ceil = 1, prime_powers_len = 0;
	for(; prime_powers_len < MAX_FACTORS; ++prime_powers_len){
		if(factors[prime_powers_len].factor){
			mask_ceil <<= 1;
		}else{
			break;
		}
	}
	--mask_ceil;
	uint64_t prime_powers[prime_powers_len];
	for(uint64_t i = 0; i < prime_powers_len; ++i){
		uint64_t p = factors[i].factor, pp = p;
		for(uint64_t j = 1; j < factors[i].power; ++j){
			p *= pp;
		}
		prime_powers[i] = p;
	}
	uint64_t ret = 1;
	for(uint64_t mask = 1; mask < mask_ceil; ++mask){
		uint64_t a = 1;
		for(uint64_t i = 0; i < prime_powers_len; ++i){
			if((1<<i)&mask){
				a *= prime_powers[i];
			}
		}
		uint64_t idempotent = make_idempotent(n, a);
		if(n + 1 - idempotent > idempotent){
			idempotent = n + 1 - idempotent;
		}
		if(idempotent > ret){
			ret = idempotent;
		}
	}
	return ret;
}

int main(){
	PrimeFactors *factorss = factorSieve(10000000);
	uint64_t s = 0;
	#pragma omp parallel for reduction(+:s)
	for(uint64_t n = 2; n <= 10000000; ++n){
		s += find_largest_idempotent(n, factorss[n]);
	}
	free(factorss);
	printf("%"PRIu64"\n", s);
}

