#include <stdio.h>
#include <inttypes.h>

uint64_t fact(uint64_t n){
	uint64_t p = 1;
	while(n > 1){
		p *= n--;
	}
	return p;
}

uint64_t pn(uint64_t t){
	uint64_t P = 0;
	for(uint64_t i = (1<<(t/2 + 1)) - 1, p, n, b; i < 1<<t; ++i){
		if(__builtin_popcount(i) <= t/2){
			continue;
		}
		b = i;
		p = n = 1;
		while(n <= t){
			if(!(b&1)){
				p *= n;
			}
			++n;
			b >>= 1;
		}
		P += p;
	}
	return P;
}

int main(void){
	printf("%"PRIu64"\n", fact(16)/pn(15));
}

