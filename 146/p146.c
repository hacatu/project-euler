#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define MAX 150000000

#define SPOKE_C 5
#define SPOKES ((uint64_t[SPOKE_C]){2, 3, 5, 7, 13})
#define WHEEL_SIZE (2*3*5*7*13)

#define DMR_PRIMES_C 8
#define DMR_PRIMES ((uint64_t[DMR_PRIMES_C]){2, 3, 5, 7, 11, 13, 17, 19})

uint64_t *offsets;
uint64_t orthSize;

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

__attribute__((constructor)) void init(void){
	char wheel[WHEEL_SIZE] = {0};
	char squareWheel[WHEEL_SIZE] = {0};
	#pragma omp parallel for
	for(uint64_t i = 1; i < WHEEL_SIZE; i += 2){
		int orth = 1;
		for(uint64_t j = 0; j < SPOKE_C; ++j){
			if(!(i%SPOKES[j])){
				orth = 0;
				break;
			}
		}
		if(orth){
			wheel[i] = 1;
		}
	}
	orthSize = 0;
	#pragma omp parallel for
	for(uint64_t i = 0; i < WHEEL_SIZE; i += 2){
		uint64_t s = i*i%WHEEL_SIZE;
		if(wheel[(s + 1)%WHEEL_SIZE] && wheel[(s + 3)%WHEEL_SIZE] && wheel[(s + 7)%WHEEL_SIZE] && wheel[(s + 9)%WHEEL_SIZE] && wheel[(s + 13)%WHEEL_SIZE] && wheel[(s + 27)%WHEEL_SIZE]){
			__sync_fetch_and_add(&orthSize, 1);//GNU extension because C11 atomic add is not supported by OpenMP
			squareWheel[i] = 1;
		}
	}
	offsets = malloc(orthSize*sizeof(uint64_t));
	for(uint64_t i = 0, j = 0; i < WHEEL_SIZE; i += 2){
		if(squareWheel[i]){
			offsets[j] = i;
			++j;
		}
	}
}

int main(void){
	uint64_t e = 0;
	#pragma omp parallel for reduction(+:e)
	for(uint64_t i = 0; i < MAX; i += WHEEL_SIZE){
		for(uint64_t j = 0, n, s; j < orthSize; ++j){
			n = (i + offsets[j]);
			if(n > MAX){
				break;
			}
			s = n*n;
			if(isPrime(s + 1) &&
			   isPrime(s + 3) &&
			   isPrime(s + 7) &&
			   isPrime(s + 9) &&
			   isPrime(s + 13) &&
			   isPrime(s + 27) &&
			   !isPrime(s + 5) &&
			   !isPrime(s + 11) &&
			   !isPrime(s + 15) &&
			   !isPrime(s + 17) &&
			   !isPrime(s + 19) &&
			   !isPrime(s + 21) &&
			   !isPrime(s + 23) &&
			   !isPrime(s + 25)){
				e += n;
			}
		}
	}
	free(offsets);
	printf("%"PRIu64"\n", e);
}

