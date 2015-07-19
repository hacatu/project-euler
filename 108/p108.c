#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#define MAX 1000000
#define MIN 1000

int main(){
	uint64_t *div = malloc((MAX + 1)*sizeof(uint64_t));
	if(!div){
		puts("malloc failed");
		return 1;
	}
	div[1] = 1;
	for(uint64_t i = 2; i <= MAX; ++i){
		div[i] = 2;
	}
	for(uint64_t d = 2; d <= sqrt(MAX); ++d){//divisor
		++div[d*d];
		for(uint64_t m = d*(d + 1); m <= MAX; m += d){//multiple
			div[m] += 2;
		}
	}
	for(uint64_t n = 5; n <= sqrt(MAX); ++n){
		if(div[n*n] >= 2*MIN){
			printf("%"PRIu64"\n", n);
			free(div);
			return 0;
		}
	}
	puts("n not found");
	free(div);
	return 1;
}

