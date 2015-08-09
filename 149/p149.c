#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define SIDE 2000
#define MAX(a, b) ({__auto_type _a = (a); __auto_type _b = (b); (_b > _a) ? _b : _a;})

int main(void){
	int64_t *s = malloc(SIDE*SIDE*sizeof(int64_t)), max;
	for(int64_t i = 0; i < 55; ++i){
		s[i] = (300007*i*i*i + 900021*i*i + 700018*i + 200007)%1000000 - 500000;
	}
	for(int64_t i = 55; i < SIDE*SIDE; ++i){
		s[i] = (s[i - 24] + s[i - 55] + 1000000)%1000000 - 500000;
	}
	max = 0;
	#pragma omp parallel for reduction(max:max)
	for(int64_t i = 0; i < SIDE; ++i){
		for(int64_t j = 0, max_here = 0; j < SIDE; ++j){//First orthagonal
			max_here = MAX(0, max_here + s[SIDE*i + j]);
			max = MAX(max, max_here);
		}
		for(int64_t j = 0, max_here = 0; j < SIDE; ++j){//second orthagonal
			max_here = MAX(0, max_here + s[i + SIDE*j]);
			max = MAX(max, max_here);
		}
		for(int64_t j = 0, max_here = 0; i + j < SIDE; ++j){//first first diagonal
			max_here = MAX(0, max_here + s[SIDE*(i + j) + j]);
			max = MAX(max, max_here);
		}
		for(int64_t j = 0, max_here = 0; i + j < SIDE; ++j){//second first diagonal
			max_here = MAX(0, max_here + s[SIDE*(j) + i + j]);
			max = MAX(max, max_here);
		}
		for(int64_t j = 0, max_here = 0; j <= i; ++j){//first second diagonal
			max_here = MAX(0, max_here + s[SIDE*(i - j) + j]);
			max = MAX(max, max_here);
		}
		for(int64_t j = 0, max_here = 0; i + j < SIDE; ++j){//second second diagonal
			max_here = MAX(0, max_here + s[SIDE*(SIDE - 1 - j) + i + j]);
			max = MAX(max, max_here);
		}
	}
	printf("%"PRId64"\n", max);
	free(s);
}

