#include <stdio.h>
#include <inttypes.h>

typedef struct{
	uint64_t a, b;
} pair_uint64_t;

int main(void){
	uint64_t sum = 0;
	for(pair_uint64_t f = {.a = 2, .b = 1}; f.a < 4000000000; f = (pair_uint64_t){.a = 3*f.a + 2*f.b, .b = 2*f.a + f.b}){
		sum += f.a;
	}
	printf("%"PRIu64"\n", sum);
}

