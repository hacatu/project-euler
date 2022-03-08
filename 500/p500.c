#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <primesieve.h>

#define POW 500500

typedef struct{
	uint64_t power, exponent;
} ppower;

static inline void sift_down(size_t len; ppower buf[static len], size_t i, size_t len){
	for(size_t a = 2*i + 1; a < len; i = a, a = 2*i + 1){
		if(a + 1 != len){
			if(buf[a].power > buf[a + 1].power){
				++a;
			}
		}
		if(buf[i].power > buf[a].power){
			ppower tmp = buf[i];
			buf[i] = buf[a];
			buf[a] = tmp;
		}else{
			break;
		}
	}
}

int main(void){
	uint64_t *primes = primesieve_generate_n_primes(POW, 2, UINT64_PRIMES);
	if(!primes){
		printf("\e[1;31mERROR: Could not allocate memory\e[0m\n");
		return 1;
	}
	ppower *ppowers = malloc(POW*sizeof(*ppowers));
	if(!ppowers){
		primesieve_free(primes);
		printf("\e[1;31mERROR: Could not allocate memory\e[0m\n");
		return 1;
	}
	for(size_t i = 0; i < POW; ++i){
		ppowers[i] = (ppower){.power=primes[i], .exponent=1};
	}
	uint64_t prod = 1;
	uint64_t psum = 0;
	uint64_t count = 0;
	size_t ppowers_len = POW;
	while(psum < POW){
		prod = prod*ppowers[0].power%500500507ULL;
		ppowers[0].power *= ppowers[0].power;
		++psum;
		if(ppowers[0].exponent == 1){
			++count;
		}
		ppowers[0].exponent <<= 1;
		if(ppowers[0].power > primes[POW - 1 - (psum - count)]){
			ppowers[0] = ppowers[--ppowers_len];
		}
		sift_down(ppowers, 0, ppowers_len);
	}
	free(ppowers);
	primesieve_free(primes);
	printf("%"PRIu64", %"PRIu64"\n", prod, psum);
}

