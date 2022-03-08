#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DMR_PRIMES (uint64_t[]){2, 7, 61}
#define DMR_PRIMES_C 3

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

int is_prime(uint64_t n){
	uint64_t s, d;//s, d | 2^s*d = n - 1
	if(n%2 == 0){
		return n == 2;
	}
	--n;
	s = __builtin_ctz(n);
	d = n >> s;
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

int cmp_uint64s(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}
	return 0;
}

uint64_t *generate_admissible_rec(uint64_t *out, uint64_t acc, uint64_t max, uint64_t nprimes, const uint64_t primes[static nprimes]){
	if(!nprimes){
		return out;
	}
	uint64_t p = *primes;
	for(acc *= p; acc < max; acc *= p){
		*out++ = acc;
		out = generate_admissible_rec(out, acc, max, nprimes - 1, primes + 1);
	}
	return out;
}

uint64_t *generate_admissible(uint64_t max, size_t *out_len, uint64_t nprimes, const uint64_t primes[static nprimes]){
	uint64_t *buf = malloc(max/2*sizeof(uint64_t));
	if(!buf){
		return NULL;
	}
	uint64_t *end = generate_admissible_rec(buf, 1, max, nprimes, primes);
	//qsort(buf, end - buf, sizeof(uint64_t), cmp_uint64s);
	*out_len = end - buf;
	return realloc(buf, (end - buf)*sizeof(uint64_t)) ?: buf;
}

uint64_t count_fortunates(size_t len, const uint64_t adms[static len]){
	char is_fortunate[252] = {};
	for(size_t i = 0; i < len; ++i){
		uint64_t n = adms[i] + 3;
		while(!is_prime(n)){
			n += 2;
		}
		n -= adms[i];
		is_fortunate[n] = 1;
	}
	uint64_t ret = 0;
	for(size_t i = 0; i < 252; ++i){
		if(is_fortunate[i]){
			ret += i;
		}
	}
	return ret;
}

int main(int argc, char **argv){
	uint64_t primes[9] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
	size_t len;
	uint64_t *adms = generate_admissible(1000000000ull, &len, 9, primes);
	uint64_t c = count_fortunates(len, adms);
	printf("Distinct fortunate numbers: %"PRIu64"\n", c);
	free(adms);
}

