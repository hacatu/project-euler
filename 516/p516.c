#define _POSIX_C_SOURCE 201805L
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*
 * 5-smooth numbers have no prime factor exceeding 5
 * Let S(L) be the sum of n <= L st phi(n) is 5-smooth
 * S(100)=3728; find S(1e12) mod 2**32
 */
#define DMR_PRIMES (uint64_t[]){2, 13, 23, 1662803}
#define DMR_PRIMES_C 4
#define MAX 1000000000000ull
//#define MAX 100

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

int isPrimeWheel(uint64_t n){
	if(!(n%2 && n%3 && n%5)){
		return n == 2 || n == 3 || n == 5;
	}
	static const uint64_t wheel_steps[] = {6, 4, 2, 4, 2, 4, 6, 2};
	for(uint64_t p = 7, i = 1; p*p <= n; p += wheel_steps[i], i = (i + 1)&7){
		if(!(n%p)){
			return 0;
		}
	}
	return 1;
}

int is_5_smooth(uint64_t n){
	n >>= __builtin_ctzll(n);
	for(; !(n%3); n /= 3);
	for(; !(n%5); n /= 5);
	return n == 1;
}

uint64_t sum_bp_subs(uint64_t tail_len, const uint64_t tail_primes[static tail_len], uint64_t base){
	uint64_t ret = base;
	for(uint64_t i = 0; i < tail_len && tail_primes[i] <= MAX/base; ++i){
		ret += sum_bp_subs(tail_len - (i + 1), tail_primes + (i + 1), base*tail_primes[i]);
	}
	return ret;
}

int cmp_uint64(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(b < a){
		return +1;
	}
	return 0;
}

int main(){
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t *primes = malloc((uint64_t)(pow(log(MAX), 3)/(log(2)*log(3)*log(5)))*sizeof(uint64_t));
	uint64_t primes_len = 0;
	for(uint64_t a = 1; a < MAX; a <<= 1){
		for(uint64_t b = a; b < MAX; b *= 3){
			for(uint64_t c = b; c < MAX; c *= 5){
				if(c == 1 || c == 2 || c == 4){
					continue;
				}
				if(isPrimeDMR(c + 1)){
					primes[primes_len++] = c + 1;
				}
			}
		}
	}
	primes = realloc(primes, primes_len*sizeof(uint64_t)) ?: primes;
	qsort(primes, primes_len, sizeof(uint64_t), cmp_uint64);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf("Got %"PRIu64" p-1 5-smooth primes in %fs.\n", primes_len,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint32_t s = 0;
	for(uint64_t a = 1; a <= MAX; a <<= 1){
		for(uint64_t b = a; b <= MAX; b *= 3){
			for(uint64_t c = b; c <= MAX; c *= 5){
				s += sum_bp_subs(primes_len, primes, c);
			}
		}
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf("%"PRIu32"\n%fs.\n", s,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec);
	free(primes);
}

