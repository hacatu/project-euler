#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../more prime sieves/factorSieve.h"

uint64_t computeRadiiSum(uint64_t n){
	uint64_t max_factors;
	void *_factorss = factorSieve(n >> 2, &max_factors);
	if(!_factorss){
		printf("\e[1;31mERROR: Could not find factors.\e[0m\n");
		return UINT64_MAX;
	}
	uint64_t (*factorss)[max_factors] = _factorss;
	for(uint64_t sqrt_bc = 4; sqrt_bc <= n >> 1; ++sqrt_bc){
		uint64_t bc = sqrt_bc*sqrt_bc;
		
	}
}

int main(){
	
}

