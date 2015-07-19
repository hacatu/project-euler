#include <stdio.h>
#include <inttypes.h>

uint64_t wcomb(uint64_t length, uint64_t block, uint64_t i){
	uint64_t nEnd = length - block*i;
	uint64_t p = nEnd + i;
	for(uint64_t n = p - 1, d = 2; n > nEnd;){
		p = p*(n--)/(d++);
	}
	return p;
}

uint64_t ways(uint64_t length, uint64_t block){
	uint64_t c = 0;
	for(uint64_t i = 0; block*i <= length; ++i){
		c += wcomb(length, block, i);
	}
	return c;
}

uint64_t cways(uint64_t length, uint64_t a, uint64_t b){
	uint64_t c = 0;
	for(uint64_t i = a; i <= b; ++i){
		c += ways(length, i);
	}
	return c - (b - a + 1);
}

int main(void){
	printf("%"PRIu64"\n", cways(50, 2, 4));
}

