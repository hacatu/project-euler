#include <stdio.h>
#include <inttypes.h>

#define MAX 100

uint64_t gcd(uint64_t a, uint64_t b){
	if(b > a){
		uint64_t temp = a;
		a = b;
		b = temp;
	}
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

void normalize(uint64_t *n, uint64_t *d){
	uint64_t g = gcd(*n, *d);
	if(g != 1){
		*n /= g;
		*d /= g;
	}
}

uint64_t e(uint64_t i){
	if(i%3 == 2){
		return (i + 1)/3<<1;
	}
	if(i){
		return 1;
	}
	return 2;
}

void addToRec(uint64_t i, uint64_t *n, uint64_t *d){
	uint64_t temp = *n;
	*n = *d;
	*d = temp;
	*n += i*(*d);
	normalize(n, d);
}

int main(){
	uint64_t n = e(MAX - 1), d = 1;
	printf("%" PRIu64 "/%" PRIu64 "\n", n, d);
	for(int i = MAX - 2; i >= 0; --i){
		addToRec(e(i), &n, &d);
		printf("%" PRIu64 "/%" PRIu64 "\n", n, d);
	}
	//printf("%" PRIu64 "/%" PRIu64 "\n", n, d);
}

