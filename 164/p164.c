#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#define LENGTH 20
#define MAX 9

uint64_t _N[LENGTH][10][10] = {0};

uint64_t N(uint64_t n, uint64_t a, uint64_t b){
	if(_N[n][a][b]){
		return _N[n][a][b];
	}
	if(!n){
		return _N[n][a][b] = 1;
	}
	uint64_t s = 0;
	for(uint64_t c = 0; a + b + c <= MAX; ++c){
		s += N(n - 1, b, c);
	}
	return _N[n][a][b] = s;
}

int main(void){
	double start = clock();
	uint64_t s = 0;
	for(uint64_t a = 1; a < 10; ++a){
		s += N(LENGTH - 1, 0, a);
	}
	printf("Found %"PRIu64" in %fs\n", s, (clock() - start)/CLOCKS_PER_SEC);
}

