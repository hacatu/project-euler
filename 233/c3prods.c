#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <primesieve.h>

//#define MAX 100000000000ull
#define MAX 38000000ull
#define MIN_C1 359125ull//17**1*13**2*5**3

int cmp_uint64s(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}
	return 0;
}

size_t upper_bound(size_t len, const uint64_t buf[static len], uint64_t k){
	if(len <= 1){
		return len ? k >= *buf : 0;
	}else if(k < buf[len/2]){
		return upper_bound(len/2, buf, k);
	}
	return len/2 + upper_bound((len + 1)/2, buf + len/2, k);
}

uint64_t sum_2_10(size_t c1primes_len, const uint64_t c1primes[c1primes_len], size_t c3prods_len, const uint64_t c3prods[c3prods_len], const uint64_t c3prod_sums[c3prods_len + 1]){
	uint64_t ret = 0;
	uint64_t p2 = 9765625ull;//5**10
	printf("Counting N with f(N) == 420 of the form N=a_1^2*5^10...\n");
	for(size_t i1 = 1; i1 <= 10; ++i1){//look at c1 primes starting at 13 and stopping at 97
		for(uint64_t c1prod = c1primes[i1]*c1primes[i1]*p2; c1prod < MAX; c1prod *= 2){
			//find the last index in c3prods so that c1prod*c3prods[i] <= MAX, then c1prod*c3prods[0], ..., c1prod*c3prods[i] have f(N) == 420
			ret += c1prod*c3prod_sums[upper_bound(c3prods_len, c3prods, MAX/c1prod + 1)];
		}
	}
	return ret;
}

uint64_t sum_3_7(size_t c1primes_len, const uint64_t c1primes[c1primes_len], size_t c3prods_len, const uint64_t c3prods[c3prods_len], const uint64_t c3prod_sums[c3prods_len + 1]){
	uint64_t ret = 0;
	for(size_t i2 = 0; i2 < c1primes_len; ++i2){
		uint64_t p2 = c1primes[i2];
		if(p2 > 17){
			break;
		}
		printf("Counting N with f(N) == 420 of the form N=a_1^3*%"PRIu64"^7...\n", p2);
		p2 = (p2*p2*p2)*(p2*p2*p2)*p2;
		for(size_t i1 = 0; i1 < c1primes_len; ++i1){
			uint64_t p1 = c1primes[i1];
			p1 = p1*p1*p1;
			if(p1*p2 > MAX){
				break;
			}
			for(uint64_t c1prod = p1*p2; c1prod < MAX; c1prod *= 2){
				ret += c1prod*c3prod_sums[upper_bound(c3prods_len, c3prods, MAX/c1prod + 1)];
			}
		}
	}
	return ret;
}

uint64_t sum_1_2_3(size_t c1primes_len, const uint64_t c1primes[c1primes_len], size_t c3prods_len, const uint64_t c3prods[c3prods_len], const uint64_t c3prod_sums[c3prods_len + 1]){
	uint64_t ret = 0;
	for(size_t i3 = 0; i3 < c1primes_len; ++i3){
		uint64_t p3 = c1primes[i3];
		p3 = p3*p3*p3;
		if(p3*325ull > MAX){//5**2*13
			break;
		}
		printf("Counting N with f(N) == 420 of the form N=a_1*a_2^2*%"PRIu64"^3...\n", p3);
		for(size_t i2 = 0; i2 < c1primes_len; ++i2){
			if(i2 == i3){
				continue;
			}
			uint64_t p2 = c1primes[i2];
			p2 = p2*p2;
			if((!i3 && 13ull*p2*p3 > MAX) || (i3 && 5ull*p2*p3 > MAX)){
				break;
			}
			for(size_t i1 = 0; i1 < c1primes_len; ++i1){
				if(i1 == i2 || i1 == i3){
					continue;
				}
				uint64_t c1prod = c1primes[i1]*p2*p3;
				if(c1prod > MAX){
					break;
				}
				for(; c1prod < MAX; c1prod *= 2){
					ret += c1prod*c3prod_sums[upper_bound(c3prods_len, c3prods, MAX/c1prod + 1)];
				}
			}
		}
	}
	return ret;
}

int main(int argc, char **argv){
	uint64_t *c3prods = malloc((MAX/MIN_C1 + 1ull)/4ull*sizeof(uint64_t));
	if(!c3prods){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	size_t len = 1;
	c3prods[0] = 1;
	size_t c3primes_len, c1primes_len;
	uint64_t *c3primes = primesieve_generate_primes(0, MAX/21125ull/* 5**3*13**2 */ + 1ull, &c3primes_len, UINT64_PRIMES);
	if(!c3primes){
		free(c3prods);
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	uint64_t *c1primes = malloc(c3primes_len*sizeof(uint64_t));
	if(!c1primes){
		free(c3prods);
		primesieve_free(c3primes);
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	{
		size_t jc3 = 0, jc1 = 0, i = 1;
		for(; i < c3primes_len; ++i){
			if(c3primes[i] > MAX/MIN_C1){
				break;
			}
			if((c3primes[i]&3) == 3){
				c3primes[jc3++] = c3primes[i];
			}else{
				c1primes[jc1++] = c3primes[i];
			}
		}
		for(; i < c3primes_len; ++i){
			if((c3primes[i]&3) == 1){
				c1primes[jc1++] = c3primes[i];
			}
		}
		c3primes_len = jc3;
		c1primes_len = jc1;
	}
	printf("Found %"PRIu64" C3 primes less than %llu\n", c3primes_len, MAX/MIN_C1 + 1ull);
	for(size_t i = 0; i < c3primes_len; ++i){
		size_t j = len;
		for(uint64_t p = c3primes[i]; p < MAX/MIN_C1 + 1ull; p *= c3primes[i]){
			for(size_t k = 0; k < len; ++k){
				size_t c = c3prods[k]*p;
				if(c >= MAX/MIN_C1 + 1ull){
					break;
				}
				c3prods[j++] = c;
			}
		}
		qsort(c3prods, j, sizeof(uint64_t), cmp_uint64s);
		len = j;
	}
	printf("Found %"PRIu64" numbers less than %llu that factor into C3 primes.\n", len, MAX/MIN_C1 + 1ull);
	primesieve_free(c3primes);
	uint64_t *c3prod_sums = malloc((len + 1)*sizeof(uint64_t));
	if(!c3prod_sums){
		free(c1primes);
		free(c3prods);
	}
	c3prod_sums[0] = 0;
	for(size_t i = 0; i < len; ++i){
		c3prod_sums[i + 1] = c3prod_sums[i] + c3prods[i];
	}
	uint64_t c = sum_2_10(c1primes_len, c1primes, len, c3prods, c3prod_sums);
	c += sum_3_7(c1primes_len, c1primes, len, c3prods, c3prod_sums);
	c += sum_1_2_3(c1primes_len, c1primes, len, c3prods, c3prod_sums);
	free(c1primes);
	free(c3prods);
	free(c3prod_sums);
	printf("Counted %"PRIu64" valid N\n", c);
}

