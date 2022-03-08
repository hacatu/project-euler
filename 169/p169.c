#include <stdio.h>
#include <inttypes.h>

typedef unsigned __int128 uint128_t;

uint128_t f(uint128_t n){
	if(!n){
		return 1;
	}
	if(n&1){
		return f(n>>1);
	}
	return f(n>>1) + f((n>>1) - 1);
}

int main(void){
	uint64_t c;
	#pragma omp parallel
	{
		c = f((uint128_t)10000000000000*(uint128_t)1000000000000);
	}
	printf("%"PRIu64"\n", c);
}

