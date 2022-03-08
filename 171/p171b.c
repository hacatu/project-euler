#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* if n < 10^20, f(n) <= 9^2*20 = 1620
 * therefore the only possible squares are 1^2, 2^2, ...,40^2
 * we can find every decreasing sequence of digit squares that add up to one and
 * calculate the number of permutations of it.
 */

typedef unsigned __int128 uint128_t;

uint128_t factorial[30] = {0};
uint64_t multichoose[21][10] = {0};
uint64_t index_part[21][21][10] = {0};

void __attribute__((__constructor__)) init_tables(void){
	for(uint128_t f = 1, i = 0; i < 30; f *= ++i){
		factorial[i] = f;
	}
	for(uint64_t l = 0; l <= 20; ++l){
		for(uint64_t i = 0; i < 10; ++i){
			multichoose[l][i] = factorial[l + i]/factorial[l]/factorial[i];
		}
	}
	for(uint64_t i = 0; i < 10; ++i){
		for(uint64_t l = 0; l <= 20; ++l){
			for(uint64_t d = 0; d <= l; ++d){
				index_part[l - d][d][i] = multichoose[l][i];
			}
		}
	}
}

uint64_t count_arrangements(char digit_counts[10], uint64_t l, uint64_t h){
	if(!l){
		return h;
	}
	uint64_t ret = 0;
	for(size_t i = 0; i < 10; ++i){
		if(digit_counts[i]){
			--digit_counts[i];
			ret = (ret + count_arrangements(digit_counts, l - 1, h*10 + i))%1000000000;
			++digit_counts[i];
		}
	}
	return ret;
}

uint64_t ways_r(uint64_t n, uint64_t l, uint64_t d, char digit_counts[10]){
	uint64_t ret = 0;
	if(d == 1){
		digit_counts[1] = n;
		digit_counts[0] = l - n;
		ret = count_arrangements(digit_counts, 20, 0);
		digit_counts[1] = 0;
		digit_counts[0] = 0;
		return ret;
	}
	//ensure (d - 1)^2*(l - c) >= n - d^2*c
	//(d - 1)^2*l - (d - 1)^2*c >= n - d^2*c
	//(d^2 - (d - 1)^2)*c >= n - (d - 1)^2*l
	//(2*d - 1)*c >= n - (d - 1)^2*l
	uint64_t c;
	if(n <= (d - 1)*(d - 1)*l){
		c = 0;
	}else{
		c = (n - (d - 1)*(d - 1)*l - 1)/(2*d - 1) + 1;
	}
	for(uint64_t i = c; i < l; ++i){
		if(l == 20){
			printf("%"PRIu64" %"PRIu64"s\n", i, d);
		}
		uint64_t m = d*d*i;
		if(m > n){
			break;
		}
		digit_counts[d] = i;
		if(m == n){
			digit_counts[0] = l - i;
			ret = (ret + count_arrangements(digit_counts, 20, 0))%1000000000;
			digit_counts[0] = 0;
			break;
		}
		ret = (ret + ways_r(n - m, l - i, d - 1, digit_counts))%1000000000;
	}
	digit_counts[d] = 0;
	return ret;
}

uint64_t ways(uint64_t n){
	return ways_r(n, 20, 9, (char[10]){0});
}

int main(void){
	uint64_t msum = 0;
	//#pragma omp parallel for reduction(+:msum)
	for(uint64_t r = 20; r <= 20; ++r){
		printf("%"PRIu64"\n", r);
		msum += ways(r*r);
	}
	msum %= 1000000000;
	printf("%"PRIu64"\n", msum);
}

