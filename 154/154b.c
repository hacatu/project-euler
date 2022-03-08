#include <stdio.h>
#include <inttypes.h>

#define LAYER 200000

uint64_t fPower2[LAYER + 1];
uint64_t fPower5[LAYER + 1];
uint64_t nPower2, nPower5;

uint64_t fPrimePower(uint64_t n, uint64_t p){
	uint64_t s = 0, d;
	while((d = n/p)){
		s += d;
		n = d;
	}
	return s;
}

uint64_t tcPrimePower(uint64_t i, uint64_t j, uint64_t k, uint64_t p){
	return fPrimePower(i + j + k, p) - fPrimePower(i, p) - fPrimePower(j, p) - fPrimePower(k, p);
}

uint64_t tcPower2(uint64_t i, uint64_t j, uint64_t k){
	return nPower2 - fPower2[i] - fPower2[j] - fPower2[k];
}

uint64_t tcPower5(uint64_t i, uint64_t j, uint64_t k){
	return nPower5 - fPower5[i] - fPower5[j] - fPower5[k];
}

void __attribute__((constructor)) init(void){
	for(uint64_t n = 0; n <= LAYER; ++n){
		fPower2[n] = fPrimePower(n, 2);
		fPower5[n] = fPrimePower(n, 5);
	}
	nPower2 = fPower2[LAYER];
	nPower5 = fPower5[LAYER];
}

int main(void){
	uint64_t c = 0;
	#pragma omp parallel for reduction(+:c)
	for(uint64_t i = 1; i <= LAYER/2; ++i){
		if(tcPower5(LAYER - 2*i, i, i) >= 12 &&
		   tcPower2(LAYER - 2*i, i, i) >= 12){
			if(3*i == LAYER){
				c += 1;
			}else{
				c += 3;
			}
		}
	}
	#pragma omp parallel for reduction(+:c)
	for(uint64_t j = 1; j < (LAYER + 2)/3; ++j){
		for(uint64_t i = j + 1, k = LAYER - i - j; i < k; ++i, --k){
			if(tcPower5(i, j, k) >= 12 &&
			   tcPower2(i, j, k) >= 12){
				c += 6;
			}
		}
	}
	printf("%"PRIu64"\n", c);
}

