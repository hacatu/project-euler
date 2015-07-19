#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

uint32_t *genPhi(uint32_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint32_t *phi = malloc(max*sizeof(uint32_t));
	if(!phi){
		return NULL;
	}
	uint64_t count = 0;
	for(uint32_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint32_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] != p){
			count += phi[p];
			continue;
		}
		for(uint32_t n = p; n < max; n += p){
			phi[n] /= p;
			phi[n] *= p - 1;
		}
		count += phi[p];
	}
	printf("%"PRIu64"\n", count);
	return phi;
}

int main(){
	uint32_t *phi = genPhi(1000001);
	free(phi);
}

