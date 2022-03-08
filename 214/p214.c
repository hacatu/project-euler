#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//const uint64_t MAX = 40000000, LEN = 25;
const uint64_t MAX = 40000000, LEN = 25;

uint64_t *phi_sieve(uint64_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint64_t *phi = malloc(max*sizeof(uint64_t));
	if(!phi){
		return NULL;
	}
	for(uint64_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint64_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] != p){
			continue;
		}
		for(uint64_t n = p; n < max; n += p){
			phi[n] /= p;
			phi[n] *= p - 1;
		}
		//printf("phi(%" PRIu32 ") = %" PRIu32 "\n", p, phi[p]);
	}
	return phi;
}

uint64_t *phi_chains(uint64_t max, const uint64_t phi[static max]){
	uint64_t *chainlen = malloc(max*sizeof(uint64_t));
	if(!chainlen){
		return NULL;
	}
	chainlen[1] = 1;
	for(uint64_t i = 2; i < max; ++i){
		chainlen[i] = chainlen[phi[i]] + 1;
	}
	return chainlen;
}

int main(){
	uint64_t *phi = phi_sieve(MAX);
	uint64_t *chainlen = phi_chains(MAX, phi);
	uint64_t s = 0;
	for(uint64_t i = 2; i < MAX; ++i){
		if(phi[i] == i - 1 && chainlen[i] == LEN){
			//printf("%"PRIu64"\n", i);
			s += i;
		}
	}
	printf("%"PRIu64"\n", s);
}

