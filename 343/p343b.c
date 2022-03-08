#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

const uint64_t MAX = 100;
uint64_t *min_factors, *lo_cache;

uint64_t *min_sieve1(uint64_t ub){
	uint64_t *res = calloc(ub, sizeof(uint64_t));
	if(!res){
		return NULL;
	}
	for(uint64_t p = 2; p < ub; ++p){
		if(res[p]){
			continue;
		}
		for(uint64_t m = p; m < ub; m += p){
			res[m] = res[m] ?: p;
		}
	}
	return res;
}

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

uint64_t f_naive(uint64_t k){
	uint64_t x = 1, y = k;
	while(y != 1){
		++x, --y;
		uint64_t d = gcd(x, y);
		if(d != 1){
			x /= d;
			y /= d;
		}
	}
	return x;
}

uint64_t *init_lo_cache(){
	uint64_t *lo_cache = calloc(MAX + 1, sizeof(uint64_t));
	if(!lo_cache){
		return NULL;
	}
	lo_cache[1] = 1;
	for(uint64_t k = 2; k <= MAX; ++k){
		uint64_t p = min_factors[k + 1];
		lo_cache[k] = k < p ? k : lo_cache[k/p];
	}
	return lo_cache;
}

uint64_t f(uint64_t k){
	if(k <= MAX){
		return lo_cache[k];
	}
	uint64_t x = 1, y = k;
	while(y != 1){
		++x, --y;
		uint64_t d = gcd(x, y);
		if(d != 1){
			x /= d;
			y /= d;
			if(x == 1){
				//printf("reduced by %"PRIu64"\n", d);
				return f(y);
			}else{
				printf("num not 1\n");
			}
		}
	}
	return x;
}

int main(){
	min_factors = min_sieve1(MAX + 2);
	lo_cache = init_lo_cache();
	uint64_t s = 1;
	//#pragma omp parallel for reduction(+:s)
	for(uint64_t k = 2; k <= MAX; ++k){
		uint64_t e = f_naive(k*k*k);
		uint64_t p = min_factors[k + 1];
		if(!p){
			printf("%"PRIu64": Oopsie\n", k);
			exit(EXIT_FAILURE);
		}
		uint64_t o = f(k*k*k);
		if(e != o){
			printf("%"PRIu64": e=%"PRIu64", o=%"PRIu64"\n", k, e, o);
		}
		s += e;
	}
	free(min_factors);
	free(lo_cache);
	printf("sum: %"PRIu64"\n", s);
}

