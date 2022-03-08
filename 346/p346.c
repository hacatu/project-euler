#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX 50ULL

int main(void){
	uint64_t *ways = malloc(((MAX - 1)/64 + 1)*sizeof(uint64_t));
	if(!ways){
		printf("\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	memset(ways, ~0, ((MAX - 1)/64 + 1)*sizeof(uint64_t));
	for(uint64_t b = 2; b < MAX - 1; ++b){
		for(uint64_t m = b + 1; m < MAX; m = m*b + 1){
			uint64_t i = m >> 5;
			uint64_t j_mask = 0x100000001ULL << (m&0x1F);
			ways[i] = (ways[i]&~j_mask) | ((ways[i]&j_mask) >> 32);
		}
	}
	//uint64_t s = 1;
	for(uint64_t n = 7; n < MAX; ++n){
		if(!(ways[n >> 5]&(1ULL << (n&0x1F)))){
			printf("%"PRIu64"\n", n);
		}
	}
	free(ways);
}