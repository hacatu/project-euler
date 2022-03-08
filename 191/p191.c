#include <stdio.h>
#include <inttypes.h>
#include <time.h>

uint64_t trinom_coeff(uint64_t n, uint64_t r1, uint64_t r2){
	uint64_t p = 1;
	for(uint64_t i = 1; i <= r1; ++i, --n){
		p = p*n/i;
	}
	for(uint64_t i = 1; i <= r2; ++i, --n){
		p = p*n/i;
	}
	return p;
}

uint64_t o_strings(uint64_t l){
	uint64_t s = 0;
	for(uint64_t i = 0; i <= l/3; ++i){
		for(uint64_t j = 0; j <= (l - 3*i)/2; ++j){
			s += trinom_coeff(l - 2*i - j, i, j)*(l - 2*i - j + 1);
		}
	}
	return s;
}

uint64_t prize_strings(uint64_t l){
	return o_strings(l) + o_strings(l - 1) + o_strings(l - 2);
}

int main(void){
	clock_t start = clock();
	uint64_t s = prize_strings(30);
	printf("%"PRIu64"\n%fs\n", s, (double)(clock() - start)/CLOCKS_PER_SEC);
}

