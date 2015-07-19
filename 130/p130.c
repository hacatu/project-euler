#include <stdio.h>
#include <inttypes.h>

#define DMR_PRIMES (uint64_t[]){2, 13, 23, 1662803}
#define DMR_PRIMES_C 4

#define N 25

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

uint64_t A(uint64_t n){
	n *= 9;
	uint64_t k = 1;
	while(powmod(10, k, n) != 1){
		++k;
	}
	return k;
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
	uint64_t n = 5, s = 0, c = 0;
	while(c < 25){
		n += 2;
		if(!(n%5)){
			continue;
		}
		if(!((n - 1)%A(n))){
			if(!isPrime(n)){
				//printf("%"PRIu64"\n", n);
				++c;
				s += n;
			}
		}
	}
	printf("%"PRIu64"\n", s);
}

