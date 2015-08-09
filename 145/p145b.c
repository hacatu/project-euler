#include <stdio.h>
#include <inttypes.h>

#define MAX 1000000000

uint64_t reverse(uint64_t n){
	uint64_t r = 0;
	while(n){
		r *= 10;
		r += n%10;
		n /= 10;
	}
	return r;
}

int odd(uint64_t n){
	while(n){
		if(!(n&1)){
			return 0;
		}
		n /= 10;
	}
	return 1;
}

int reversible(uint64_t n){
	if(!(n%10)){
		return 0;
	}
	return odd(n + reverse(n));
}

int main(void){
	int c = 0;
	#pragma omp parallel for reduction(+:c)
	for(uint64_t n = 1; n < MAX; ++n){
		if(reversible(n)){
			++c;
		}
	}
	printf("%i\n", c);
}

