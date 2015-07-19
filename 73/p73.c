#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

uint32_t gcd(uint32_t a, uint32_t b){
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

uint32_t *genPhi(uint32_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint32_t *phi = malloc(max*sizeof(uint32_t));
	if(!phi){
		return NULL;
	}
	uint32_t count = 0;
	for(uint32_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint32_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] != p){//p is composite and phi[p] is computed already
			count += phi[p]>>1;
			//count n coprime p, n <= p/3
			for(uint32_t n = 1; n <= p/3; ++n){
				if(gcd(p, n) == 1){
					--count;
				}
			}
			continue;
		}
		for(uint32_t n = p; n < max; n += p){
			phi[n] /= p;
			phi[n] *= p - 1;
		}
		count += phi[p]>>1;
		for(uint32_t n = 1; n <= p/3; ++n){
			if(gcd(p, n) == 1){
				--count;
			}
		}
	}
	//printf("%"PRIu32"->%"PRIu32"\n", max - 1, phi[max - 1]);
	printf("%"PRIu32"\n", count);
	return phi;
}

int main(){
	uint32_t *phi = genPhi(12001);
	free(phi);
}

