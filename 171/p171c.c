#include <stdio.h>
#include <inttypes.h>

#define LEN 20
#define BASE 10
#define MAX 40
#define MOD 1000000000

int main(void){
	uint64_t heads[LEN][MAX*MAX + 1] = {0};
	uint64_t ways[LEN][MAX*MAX +1] = {0};
	for(uint64_t d = 0; d < BASE; ++d){
		heads[0][d*d] = d;
		ways[0][d*d] = 1;
	}
	for(size_t i = 1; i < LEN; ++i){
		for(uint64_t s = 0; s <= i*(BASE - 1)*(BASE - 1) && s <= MAX*MAX; ++s){
			for(uint64_t d = 0; d <= BASE - 1 && d*d <= MAX*MAX - s; ++d){
				heads[i][s + d*d] = (heads[i][s + d*d] + heads[i - 1][s]*BASE + d*ways[i - 1][s])%MOD;
				ways[i][s + d*d] = (ways[i][s + d*d] + ways[i - 1][s])%MOD;
			}
		}
	}
	uint64_t s = 0;
	for(uint64_t i = 1; i <= MAX; ++i){
		s = (s + heads[LEN - 1][i*i])%MOD;
	}
	printf("%"PRIu64"\n", s);
}

