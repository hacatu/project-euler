#include <stdio.h>
#include <inttypes.h>

#define MAX_EXP 30
#define TRI(n) ((n)*((n) + 1) >> 1)

uint64_t *combinations[MAX_EXP + 1];

void __attribute__((constructor)) init_combinations(void){
	static uint64_t _combinations[TRI(MAX_EXP + 1)];
	combinations[0] = _combinations + TRI(0);
	combinations[0][0] = 1;
	for(uint64_t n = 1; n <= MAX_EXP; ++n){
		combinations[n] = _combinations + TRI(n);
		combinations[n][0] = combinations[n][n] = 1;
		for(uint64_t r = 1; r < n; ++r){
			combinations[n][r] = combinations[n - 1][r - 1] + combinations[n - 1][r];
		}
	}
}

uint64_t X0(void){
	uint64_t ret = 0;
	for(uint64_t a = 0; a < MAX_EXP; ++a){
		for(uint64_t b = 0; b <= a >> 1; ++b){
			ret += combinations[a - b][b];
		}
	}
	return ret + 1;
}

int main(void){
	printf("%"PRIu64"\n", X0());
}

