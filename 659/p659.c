#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <primesieve.h>

#define MAX 10000000ull
#define MODULUS 1000000000000000000ull

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

uint64_t gcd(uint64_t a, uint64_t b){
	if(b > a){
		uint64_t temp = a;
		a = b;
		b = temp;
	}
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

uint64_t largest_prime_factor_td(uint64_t n, size_t size, const uint64_t primes[static size]){
	uint64_t ret = 1;
	for(size_t i = 0; i < size && n != 1; ++i){
		if(n%primes[i] == 0){
			ret = primes[i];
			do{
				n /= primes[i];
			}while(n%primes[i] == 0);
		}
	}
	return ret;
}

int64_t jacobik(uint64_t n, uint64_t k){
	int64_t j = 1;
	while(1){
		uint64_t s = __builtin_ctzll(n);
		uint64_t q = n >> s;
		if((s&1) && ((k&7) == 3 || ((k&7) == 5))){
			j = -j;
		}
		if(q == 1){
			return j;
		}else if(q == k - 1){
			return (k&3)==1 ? j : -j;
		}else if((q&2) && (k&2)){
			j = -j;
		}
		n = k%q;
		k = q;
	}
}

uint32_t randint(uint32_t a, uint32_t b){
	uint32_t l = b - a, m = RAND_MAX/l*l, r;
	while((r = rand()) >= m);
	return r%l + a;
}

uint64_t random_nonresidue(uint64_t p){
	while(1){
		uint64_t z = randint(2, p - 1);
		if(jacobik(z, p) == -1){
			return z;
		}
	}
}

uint64_t sqrt_n1_shanks(uint64_t p){
	uint64_t s = __builtin_ctzll(p-1);
	uint64_t q = (p-1) >> s;//p = q*2^s + 1
	uint64_t z = random_nonresidue(p);
	//printf("trying \"nonresidue\" %"PRIu64"\n", z);
	uint64_t m = s;
	uint64_t c = powmod(z, q, p);
	uint64_t t = p - 1;//(-1)^Q
	uint64_t r = q&2 ? 1 : p - 1;//if q&2 then (-1)^((q+1)/2) = 1 else = -1
	do{
		uint64_t i = 1;
		for(uint64_t s = t*t%p; s != 1; s = s*s%p,++i);
		uint64_t b = c;
		for(uint64_t j = 0; j < m - i - 1; ++j){
			b = b*b%p;
		}
		m = i;
		c = b*b%p;
		t = t*c%p;
		r = r*b%p;
	}while(t != 1);
	return r;
}

uint64_t sqrt_n1_mod(uint64_t p){
	uint64_t r;
	if((p&7) == 5){
		r = powmod(2, p >> 2, p);
	}else{//if(m=64-__builtin_clzll(p),s=__builtin_ctzll(p-1),8*m+20 <= s*(s-1))
		r = sqrt_n1_shanks(p);
	}//else use cipolla except I won't actually implement it
	return r;
}

int64_t mmi(int64_t a, int64_t b){
	int64_t r0 = b, r1 = a;
	int64_t s0 = 1, s1 = 0;
	int64_t t0 = 0, t1 = 1;
	while(r1){
		int64_t q = r0/r1, t;
		t = r1;
		r1 = r0 - q*r1;
		r0 = t;
		t = s1;
		s1 = s0 - q*s1;
		s0 = t;
		t = t1;
		t1 = t0 - q*t1;
		t0 = t;
	}
	return t0 < 0 ? t0 + b : t0;
}

int main(){
	size_t size;
	uint64_t *primes = primesieve_generate_primes(1, 2*MAX, &size, UINT64_PRIMES);
	uint64_t *large_factors = malloc((MAX + 1)*sizeof(uint64_t));
	uint64_t *max_factors = malloc((MAX + 1)*sizeof(uint64_t));
	for(uint64_t k = 1; k <= MAX; ++k){
		large_factors[k] = 4*k*k + 1;
		max_factors[k] = 4*k*k + 1;
	}
	for(size_t i = 0; i < size; ++i){
		uint64_t p = primes[i];
		if((p&7) != 1 && (p&7) != 5){
			continue;
		}
		//printf("p:%8"PRIu64"\n", p);
		uint64_t r = sqrt_n1_mod(p)*mmi(2, p) % p;
		uint64_t a = mmi(8*r % p, p);
		uint64_t w = p;
		while(1){
			/*if((4*(unsigned __int128)r*r + 1)%w){
				printf("Oopsed with w=%"PRIu64",r=%"PRIu64"!\n", w, r);
				exit(119);
			}*/
			uint64_t s = r <= w - r ? r : w - r;
			uint64_t m = 0;
			for(; m + w - s <= MAX; m += w){
				if(w == p){//obviously this gets lifted out
					max_factors[m + s] = p;
					max_factors[m + w - s] = p;
				}
				large_factors[m + s] /= p;
				large_factors[m + w - s] /= p;
			}
			if(m + s <= MAX){
				if(w == p){
					max_factors[m + s] = p;
				}
				large_factors[m + s] /= p;
			}
			if((unsigned __int128)w*p > (unsigned __int128)4*MAX*MAX + 1){
				break;
			}
			w *= p;
			r = w - ((4*(unsigned __int128)r*r + 1)*a - r)%w;//(r - (4*r*r + 1)/_w*a*_w) mod w
		}
	}
	primesieve_free(primes);
	uint64_t sum = 0;
	/*
	for(uint64_t k = 1; k <= MAX; ++k){
		sum += largest_prime_factor_td(4*k*k + 1, pyPrimes_len, pyPrimes);
		if(k%25000 == 0){
			sum %= MODULUS;//modulus + \sum_{k=max-25000}^{max}4k^2+1 < 2**64
			printf("%8"PRIu64":%17"PRIu64"\n", k, sum);
		}
	}*/
	for(uint64_t k = 1; k <= MAX; ++k){
		sum += max_factors[k] >= large_factors[k] ? max_factors[k] : large_factors[k];
		if(k%25000 == 0){
			sum %= MODULUS;//modulus + \sum_{k=max-25000}^{max}4k^2+1 < 2**64
		}
	}
	free(large_factors);
	free(max_factors);
	printf("sum: %"PRIu64"\n", sum);
}

