#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define MAX 100000000UL

const uint64_t self_pow[] = {1UL, 1UL, 4UL, 27UL, 256UL, 3125UL, 46656UL, 823543UL, 16777216UL, 387420489UL, 10000000000UL, 285311670611UL, 8916100448256UL, 302875106592253UL, 11112006825558016UL, 437893890380859375UL};

uint64_t min_m(uint64_t e, uint64_t n){
	uint64_t p = 1, s = 1, t = n - 1, ret = 0;
	while(t < e){
		p *= n, s += p, t += s*(n - 1);
	}
	while(p){
		uint64_t max_tail = t - s*(n - 1);
		uint64_t d = (e - max_tail + s - 1)/s;
		ret += d*p;
		if(d*s >= e){
			return ret*n;
		}
		e -= d*s;
		t -= s*(n - 1), s -= p, p /= n;
	}
	return ret*n;
}

uint64_t fact_sieve(uint64_t max){
	uint64_t lin_rng_a;
	for(lin_rng_a = 1; lin_rng_a < 16 && self_pow[lin_rng_a] <= max; ++lin_rng_a);
	--lin_rng_a;
	if(lin_rng_a%2 == 0 && lin_rng_a != 2){
		--lin_rng_a;
	}
	printf("%"PRIu64"\n", lin_rng_a);
	//uint64_t min_fact_lut[lin_rng_a + 1][64 - __builtin_clzll(max)];
	uint64_t *min_facts = calloc(max, sizeof(uint64_t));
	if(!min_facts){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate %fG memory.\e[0m\n", max*sizeof(uint64_t)/1024./1024/1024);
		exit(1);
	}
	uint64_t s = 0;
	for(uint64_t n = 2; n <= lin_rng_a; ++n){
		if(min_facts[n]){
			s += min_facts[n];
			//printf("%"PRIu64": %"PRIu64"\n", n, min_facts[n]);
			continue;
		}
		for(uint64_t p = n, e = 1; p <= max; p *= n, ++e){
			uint64_t min_fact = e*n;
			if(e > n){
				min_fact = min_m(e, n);
			}
			for(uint64_t m = p; m <= max; m += p){
				if(min_fact > min_facts[m]){
					min_facts[m] = min_fact;
				}
			}
		}
		//printf("%"PRIu64": %"PRIu64"\n", n, min_facts[n]);
		s += min_facts[n];
	}
	for(uint64_t n = lin_rng_a + 1; n <= max; n++){
		if(min_facts[n]){
			s += min_facts[n];
			//printf("%"PRIu64": %"PRIu64"\n", n, min_facts[n]);
			continue;
		}
		for(uint64_t p = n, e = 1; p <= max; p *= n, ++e){
			if(p == 81){
			}
			uint64_t min_fact = e*n;
			for(uint64_t m = p; m <= max; m += p){
				if(min_fact > min_facts[m]){
					min_facts[m] = min_fact;
				}
			}
		}
		//printf("%"PRIu64": %"PRIu64"\n", n, min_facts[n]);
		s += min_facts[n];
	}
	return s;
}

int main(void){
	printf("%"PRIu64"\n", fact_sieve(MAX));
}

