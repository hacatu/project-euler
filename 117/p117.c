#include <stdio.h>
#include <inttypes.h>

#define MAX 50

int main(void){
	uint64_t a = 8;
	for(uint64_t b = 4, c = 2, d = 1, i = 4, t; i < MAX; ++i){
		t = a;
		a += b + c + d;
		d = c;
		c = b;
		b = t;
	}
	printf("%"PRIu64"\n", a);
}

