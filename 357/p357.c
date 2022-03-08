#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <primesieve.h>

#define MAX 100000000UL
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

/*
 * For odd n, n + 1 is even and hence n not prime generating (except 1)
 * For powers n = x^p, x + x^(p-1) is not prime (except for 1)
 * For n = p*p*a, p + p*a is not prime so n must be squarefree
 * n must be a squarefree integer such that n%2 != 1 and n%3 != 2
 * n%6 <- [0, 4]
 */

uint64_t count_pgens(uint64_t *primes_a, uint64_t *primes_b, size_t factors_len, uint64_t factors[static 8], uint64_t p){
	uint64_t ret = 0;
	for(uint64_t *primes_it = primes_a, n; primes_it < primes_b; ++primes_it){
		n = p * *primes_it;
		if(n >= MAX){
			break;
		}
		factors[factors_len] = *primes_it;
		if(!(n%6&3)){//n%6 <- [0, 4]
			for(uint64_t d = 1, i = 0, g = 0;;){
				if(!is_prime(d + n/d)){
					goto NOT_PRIME_GEN;
				}
				++i;
				if(i >= 2 << factors_len){
					break;
				}
				g ^= i&-i;
				if(g&i&-i){
					d *= factors[__builtin_ctzll(i)];
				}else{
					d /= factors[__builtin_ctzll(i)];
				}
			}
			//printf("%"PRIu64"\n", n);
			ret += n;
			NOT_PRIME_GEN:;
		}
		ret += count_pgens(primes_it + 1, primes_b, factors_len + 1, factors, n);
	}
	return ret;
}

int main(void){
	size_t primes_len;
	uint64_t *primes = primesieve_generate_primes(3, MAX, &primes_len, UINT64_PRIMES);
	if(!primes){
		fprintf(stderr, "ERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	uint64_t factors[8];
	//puts("1\n2");
	printf("%"PRIu64"\n", 1 + 2 + count_pgens(primes, primes + primes_len, 0, factors, 2));
	primesieve_free(primes);
}

