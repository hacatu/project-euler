#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

uint64_t *genPhi(uint64_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint64_t *phi = malloc(max*sizeof(uint64_t));
	if(!phi){
		return NULL;
	}
	for(uint64_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint64_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] == p){
			for(uint32_t n = p; n < max; n += p){
				phi[n] /= p;
				phi[n] *= p - 1;
			}
		}
	}
	return phi;
}

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

//if a and n are coprime, a^phi(n) = 1 mod n
//so to calculate a^b mod n
//a^(b/phi(n)*phi(n) + b%phi(n)) = a^b%phi(n) mod n
uint64_t tetmod(uint64_t a, uint64_t b, uint64_t n, uint64_t *phi){
	if(b == 1){
		return a;
	}
	return powmod(a, tetmod(a, b - 1, phi[n], phi), n);
}

int main(void){
	clock_t start_time = clock(), phi_time, tetmod_time;
	uint64_t *phi = genPhi(100000001);
	phi_time = clock();
	uint64_t result = tetmod(1777, 1855, 100000000, phi);
	tetmod_time = clock();
	printf("Calculated %"PRIu64" in %fs (%fs calculating phi table and %fs calculating tetmod).\n", result,
	       (double)(tetmod_time - start_time)/CLOCKS_PER_SEC,
	       (double)(phi_time - start_time)/CLOCKS_PER_SEC,
	       (double)(tetmod_time - phi_time)/CLOCKS_PER_SEC);
	free(phi);
}

