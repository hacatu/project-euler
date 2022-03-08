#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#define LENGTH 20
#define MAX 9

uint64_t _N[LENGTH][10][10] = {0};

uint64_t N(uint64_t n, uint64_t a, uint64_t b){
	uint64_t s = 0;
	for(uint64_t c = 0; a + b + c <= MAX; ++c){
		s += _N[n - 1][b][c];
	}
	return s;
}

int main(void){
	double start = clock();
	for(uint64_t a = 0; a < 10; ++a){
		for(uint64_t b = 0; a + b <= MAX; ++b){
			_N[0][a][b] = 1;
		}
	}
	for(uint64_t n = 1; n < LENGTH - 1; ++n){
		for(uint64_t a = 0; a < 10; ++a){
			for(uint64_t b = 0; a + b <= MAX; ++b){
				_N[n][a][b] = N(n, a, b);
			}
		}
	}
	uint64_t s = 0;
	for(uint64_t a = 1; a < 10; ++a){
		s += N(LENGTH - 1, 0, a);
	}
	printf("Found %"PRIu64" in %fs\n", s, (clock() - start)/CLOCKS_PER_SEC);
}

