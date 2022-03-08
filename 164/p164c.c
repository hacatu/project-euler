#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#define LENGTH 20
#define MAX 9

uint64_t _prev[10][10], (*prev)[10] = _prev, _curr[10][10], (*curr)[10] = _curr;

uint64_t N(uint64_t a, uint64_t b){
	uint64_t s = 0;
	for(uint64_t c = 0; a + b + c <= MAX; ++c){
		s += prev[b][c];
	}
	return s;
}

int main(void){
	double start = clock();
	for(uint64_t a = 0; a < 10; ++a){
		for(uint64_t b = 0; a + b <= MAX; ++b){
			prev[a][b] = MAX - a - b + 1;
		}
	}
	for(uint64_t n = 2; n < LENGTH - 1; ++n){
		for(uint64_t a = 0; a < 10; ++a){
			for(uint64_t b = 0; a + b <= MAX; ++b){
				curr[a][b] = N(a, b);
			}
		}
		uint64_t (*t)[10] = prev;
		prev = curr;
		curr = t;
	}
	uint64_t s = 0;
	for(uint64_t a = 1; a < 10; ++a){
		s += N(0, a);
	}
	printf("Found %"PRIu64" in %fs\n", s, (clock() - start)/CLOCKS_PER_SEC);
}

