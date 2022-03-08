#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define ROWS 51

int squarefree(uint64_t n, uint64_t m){
	for(uint64_t i = 2; i < m; ++i){
		if(n%(i*i) == 0){
			return 0;
		}
	}
	return 1;
}

int cmp_uint64_ts(const void *a, const void *b){
	if(*(uint64_t*)a < *(uint64_t*)b){
		return -1;
	}else if(*(uint64_t*)a > *(uint64_t*)b){
		return 1;
	}
	return 0;
}

int main(void){
	uint64_t row_a[25] = {3}, row_b[26] = {0}, ss[12*25*2 + 1] = {0};
	uint64_t s = 6, w = 0;
	for(uint64_t row = 4, n;;){
		n = row_b[0] = row;
		if(squarefree(n, row)){
			s += n;
		}
		for(uint64_t i = 1; 2*i + 2 < row; ++i){
			n = row_b[i] = row_a[i - 1] + row_a[i];
			if(n >= ROWS && squarefree(n, row)){
				ss[w++] = n;
			}
		}
		n = row_b[row/2 - 1] = 2*row_a[row/2 - 2];
		if(n >= ROWS && squarefree(n, row)){
			s += n;
		}
		if(++row >= ROWS)break;
		n = row_a[0] = row;
		if(squarefree(n, row)){
			ss[w++] = n;
		}
		for(uint64_t i = 1; 2*i + 2 < row; ++i){
			n = row_a[i] = row_b[i - 1] + row_b[i];
			if(n >= ROWS && squarefree(n, row)){
				ss[w++] = n;
			}
		}
		if(++row >= ROWS)break;
	}
	qsort(ss, w, sizeof(uint64_t), cmp_uint64_ts);
	for(uint64_t i = 0; i < w; ++i){
		if(ss[i] != ss[i + 1]){
			s += ss[i];
		}
	}
	printf("%"PRIu64"\n", s);
}