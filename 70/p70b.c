#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

uint8_t digitsa[10];

uint8_t digitsb[10];

void countDigits(uint32_t a, uint8_t *digits){
	memset(digits, 0, 10*sizeof(uint8_t));
	while(a){
		++(digits[a%10]);
		a /= 10;
	}
}

int cmpDigits(uint32_t a, uint32_t b){
	countDigits(a, digitsa);
	countDigits(b, digitsb);
	return memcmp(digitsa, digitsb, 10*sizeof(uint8_t));
}

uint32_t *genPhi(uint32_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint32_t *phi = malloc(max*sizeof(uint32_t));
	if(!phi){
		return NULL;
	}
	uint32_t minp = 87109;
	double min = 87109./79180.;
	for(uint32_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint32_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] != p){
			//printf("phi(%" PRIu32 ") = %" PRIu32 "\n", p, phi[p]);
			if(!cmpDigits(p, phi[p])){
				//printf("phi(%" PRIu32 ") = %" PRIu32 "\n", p, phi[p]);
				if(p/(double)phi[p] < min){
					min = p/(double)phi[p];
					minp = p;
					//printf("8888m:%" PRIu32 "\n", p);
				}
			}
			continue;
		}
		for(uint32_t n = p; n < max; n += p){
			phi[n] /= p;
			phi[n] *= p - 1;
		}
		//printf("phi(%" PRIu32 ") = %" PRIu32 "\n", p, phi[p]);
	}
	printf("minp:%" PRIu32 "\n", minp);
	return phi;
}

int main(){
	uint32_t *phi = genPhi(10000000);
	free(phi);
}

