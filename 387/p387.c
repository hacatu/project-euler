#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define DMR_PRIMES (uint64_t[]){2, 3, 5, 7, 11, 13, 17}
#define DMR_PRIMES_C 7

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
	if(n == 1){
		return 0;
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

uint64_t ssrthp(uint64_t t, uint64_t s, uint64_t l){
	uint64_t ret = 0;
	for(uint64_t d = s ? 0 : 1; d < 10; ++d){
		uint64_t t2 = t*10 + d, s2 = s + d, t3;
		if(t2%s2){
			continue;
		}//otherwise t2 is Harshad
		if(is_prime(t2/s2)){//t2 is strong Harshad
			if(s2%3 != 2){
				if(is_prime((t3 = t2*10 + 1))){
					//printf("%"PRIu64"\n", t3);
					ret += t3;
				}
				if(is_prime((t3 = t2*10 + 7))){
					//printf("%"PRIu64"\n", t3);
					ret += t3;
				}
			}
			if(s2%3 != 0){
				if(is_prime((t3 = t2*10 + 3))){
					//printf("%"PRIu64"\n", t3);
					ret += t3;
				}
				if(is_prime((t3 = t2*10 + 9))){
					//printf("%"PRIu64"\n", t3);
					ret += t3;
				}
			}
		}
		if(l > 1){
			ret += ssrthp(t2, s2, l - 1);
		}
	}
	return ret;
}

int main(void){
	printf("%"PRIu64"\n", ssrthp(0, 0, 13));
}

