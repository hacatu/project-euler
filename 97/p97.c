#include <stdio.h>
#include <inttypes.h>

int main(){
	uint64_t a = 28433;
	for(uint64_t p = 0; p < 7830457; ++p){
		a <<= 1;
		a %= 10000000000;
	}
	++a;
	printf("%" PRIu64 "\n", a);
}

