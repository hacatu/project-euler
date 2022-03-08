#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <gmp.h>

#define LENGTH 100000
#define MAX 9

mpz_t _prev[10][10], (*prev)[10] = _prev, _curr[10][10], (*curr)[10] = _curr;

void N(uint64_t a, uint64_t b){
	mpz_set_ui(curr[a][b], 0);
	for(uint64_t c = 0; a + b + c <= MAX; ++c){
		mpz_add(curr[a][b], curr[a][b], prev[b][c]);
	}
}

int main(void){
	double start = clock();
	for(uint64_t a = 0; a < 10; ++a){
		for(uint64_t b = 0; a + b <= MAX; ++b){
			mpz_init(prev[a][b]);
			mpz_set_ui(prev[a][b], MAX - a - b + 1);
			mpz_init(curr[a][b]);
		}
	}
	for(uint64_t n = 2; n < LENGTH - 1; ++n){
		for(uint64_t a = 0; a < 10; ++a){
			for(uint64_t b = 0; a + b <= MAX; ++b){
				N(a, b);
			}
		}
		mpz_t (*t)[10] = prev;
		prev = curr;
		curr = t;
	}
	mpz_t s;
	mpz_init(s);
	for(uint64_t a = 1; a < 10; ++a){
		N(0, a);
		mpz_add(s, s, curr[0][a]);
	}
	gmp_printf("Found %Z in %fs\n", s, (clock() - start)/CLOCKS_PER_SEC);
}

