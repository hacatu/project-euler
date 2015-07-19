#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX 100000

typedef struct{
	uint64_t n, rad;
} nrad;

nrad *radSieve(uint64_t max){
	nrad *rad = calloc(max + 1, sizeof(nrad));
	if(!rad){
		return NULL;
	}
	rad->n = max;
	for(uint64_t n = 1; n <= max; ++n){
		rad[n] = (nrad){n, 1};
	}
	for(uint64_t n = 2; n <= max; ++n){
		if(rad[n].rad != 1){//n is not prime
			continue;
		}
		for(uint64_t m = n; m <= max; m += n){
			rad[m].rad *= n;
		}
	}
	return rad;
}

int compare_nrad_fn(nrad *a, nrad *b){
	if(a->rad < b->rad){
		return -1;
	}
	if(a->rad > b->rad){
		return 1;
	}
	return 0;
}

int main(void){
	nrad *rad = radSieve(MAX);
	qsort(rad + 1, rad->n, sizeof(nrad), (__compar_fn_t)compare_nrad_fn);
	printf("%"PRIu64"\n", rad[10000].n);
	free(rad);
}

