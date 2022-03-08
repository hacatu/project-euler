#include <stdio.h>
#include <inttypes.h>

#define n 10

uint64_t gcd(uint64_t a, uint64_t b){
	while(1){
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
		a %= b;
	}
}

int main(void){
	uint64_t s = 0;
	for(uint64_t i = 1; i <= n; ++i){
		s += n/i*i;
	}
	for(int k = 1; 2*k <= n; ++k){
		s += n/(2*k)*k*2;
	}
	for(uint64_t a = 2; a*a < n; ++a){
		for(uint64_t b = 1; b < a && a*a + b*b <= n; ++b){
			if(gcd(a, b) == 1){
				int c = a*a + b*b;
				for(int k = 1; c*k <= n; ++k){
					s += n/(c*k)*2*k*(a + b);
				}
			}
		}
	}
	printf("%"PRIu64"\n", s);
}

