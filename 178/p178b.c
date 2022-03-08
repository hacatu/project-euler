#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define LEN 20

uint64_t ways(){
	uint64_t (*_f)[10][1024] = calloc(LEN*10*1024, sizeof(uint64_t));
	if(!_f){
		return 0;
	}
	for(uint64_t d = 0; d < 10; ++d){
		_f[0][d][1023] = 1;
	}
	for(uint64_t n = 1; n < LEN; ++n){
		for(uint64_t v = 0; v < 1024; ++v){
			_f[n][0][v] = _f[n-1][1][v | 1];
			for(uint64_t d = 1; d < 9; ++d){
				_f[n][d][v] = _f[n-1][d-1][v | (1 << d)] + _f[n-1][d+1][v | (1 << d)];
			}
			_f[n][9][v] = _f[n-1][8][v | (1 << 9)];
		}
	}
	uint64_t res = 0;
	for(uint64_t d = 1; d < 10; ++d){
		for(uint64_t n = 4 < d ? 19 - d : 10 + d; n < LEN; ++n){
			res += _f[n][d][0];
		}
	}
	free(_f);
	return res;
}

int main(){
	printf("There are %"PRIu64" pandigital step numbers with fewer than %i digits.\n", ways(), LEN);
}

