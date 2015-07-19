#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

/* n^3 + n^2*p = n^2*(n + p)
 * For this to be a perfect cube, n^2 and hence n must be a perfect cube, and n + p must be a perfect cube.
 * Therefore, p is the difference between two perfect cubes ((n + p) and n).  The difference between two perfect cubes is:
 * a^3 - b^3 = (a - b)*(a^2 + a*b + b^2).  Since this must be prime, a and b must be consecutive so a - b is 1.
 */

#define MAX 1000000

#define DMR_PRIMES (uint64_t[]){2, 3}
#define DMR_PRIMES_C 2

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

int main(){
	clock_t start = clock();
	int c = 0;
	for(uint64_t a = 1, p;;){
		p = a*a*a;
		++a;
		p = a*a*a - p;
		if(p >= 1000000){
			break;
		}
		if(isPrime(p)){
			++c;
		}
	}
	printf("found %i primes in %f seconds.\n", c, (double)(clock() - start)/CLOCKS_PER_SEC);
}

