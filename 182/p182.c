#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <primesieve.h>

const uint64_t p = 1009, q = 3643;
const uint64_t n = p*q;
const uint64_t phi = (p-1)*(q-1);
const uint64_t lam = 611856;

size_t primes_len;
size_t loprimes_len;
uint64_t *primes;
uint64_t *loprimes;

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

int main(){
	/*
	primes = primesieve_generate_primes(0, lam, &primes_len, UINT64_PRIMES);
	loprimes = malloc(primes_len*sizeof(uint64_t));
	for(uint64_t i = 0; i < primes_len; ++i){
		uint64_t a = primes[i];
		while(a*primes[i] < lam){
			a *= primes[i];
		}
		int has_sln = 0;
		for(uint64_t m = 2; m < p; ++m){
			if(powmod(m, a, p) == 1){
				has_sln = 1;
				break;
			}
		}
		if(!has_sln){
			for(uint64_t m = 2; m < q; ++m){
				if(powmod(m, a, q) == 1){
					has_sln = 1;
					break;
				}
			}
		}
		if(!has_sln){
			loprimes[loprimes_len++] = a;
		}else{
			printf("%"PRIu64"\n", a);
		}
	}
	loprimes = realloc(loprimes, loprimes_len*sizeof(uint64_t));
	primesieve_free(primes);
	free(loprimes);
	printf("There are %zu loprimes and %zu hiprimes\n", loprimes_len, primes_len - loprimes_len);
	*/
	//uint64_t a = 1ull << (63 - __builtin_clzll(lam));
	/*
	uint64_t a = 4;
	printf("%"PRIu64" < %"PRIu64"\n", a, lam);
	printf("p solutions:\n");
	for(uint64_t m = 2; m < p; ++m){
		if(powmod(m, a, p) == 1){
			printf("%"PRIu64"\n", m);
		}
	}
	printf("q solutions:\n");
	for(uint64_t m = 2; m < q; ++m){
		if(powmod(m, a, q) == 1){
			printf("%"PRIu64"\n", m);
		}
	}
	*/
	uint64_t s = 0;
	for(uint64_t e = 11; e < phi; e += 12){
		if(e%7 > 1 && e%607 > 1){
			s += e;
		}
	}
	printf("%"PRIu64"\n", s);
	s = 0;
	for(uint64_t e = 0; e < phi/12; ++e){
		s += 12*e + 11;
	}
	for(uint64_t e = 0; e < phi/12; ++e){
		if(e%7 == 2 || e%7 == 5){
			s -= 12*e + 11;
		}
	}
	for(uint64_t e = 0; e < phi/12; ++e){
		if(e%607 == 252 || e%607 == 505){
			s -= 12*e + 11;
		}
	}
	for(uint64_t e = 0; e < phi/12; ++e){
		if(e%4249 == 859 || e%4249 == 2326 || e%4249 == 3540 || e%4249 == 3894){
			s += 12*e + 11;
		}
	}
	printf("%"PRIu64"\n", s);
	s = 0;
	for(uint64_t e = 0; e < phi/12; ++e){
		s += 12*e + 11;
	}
	for(uint64_t e = 0; e < phi/84; ++e){
		s -= 168*e + 106;
	}
	for(uint64_t e = 0; e < phi/7284; ++e){
		s -= 14568*e + 9106;
	}
	for(uint64_t e = 0; e < phi/50988; ++e){
		s += 203952*e + 127472;
	}
	printf("%"PRIu64"\n", s);
	s = phi/12*11 - phi/84*106 - phi/7284*9106 + phi/50988*127472
	+ 6*(phi/12 - 1)*phi/12
	- 84*(phi/84 - 1)*phi/84
	- 7284*(phi/7284 - 1)*phi/7284
	+ 101976*(phi/50988 - 1)*phi/50988;
	printf("%"PRIu64"\n", s);
	s = phi*(3025*phi + 41266)/101976;
	printf("%"PRIu64"\n", s);
}


