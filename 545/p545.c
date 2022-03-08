#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define DMR_PRIMES (uint64_t[]){2, 13, 23, 1662803}
#define DMR_PRIMES_C 4

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

int isPrime(uint64_t n){
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
	uint64_t f = 1;
	for(uint64_t n = 616; n > 615; n += 308){
		for(uint64_t d = 2; d < 6; ++d){
			if(n%d == 0){
				if(isPrime(n/d + 1)){
					if(n/d != 22 && n/d != 28){
						goto CONTINUE_NLOOP;
					}
				}
			}
		}
		for(uint64_t d = 6; d <= sqrtl(n); ++d){
			if(n%d == 0){
				if(isPrime(d + 1)){
					if(d != 22 && d != 28){
						goto CONTINUE_NLOOP;
					}
				}
				if(isPrime(n/d + 1)){
					if(n/d != 22 && n/d != 28){
						goto CONTINUE_NLOOP;
					}
				}
			}
		}
		if(++f == 100000){
			printf("%"PRIu64"\n", n);
			break;
		}
		CONTINUE_NLOOP:;
	}
}

