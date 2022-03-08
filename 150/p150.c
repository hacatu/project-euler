#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define SIDE 1000

#define MIN(a, b) ({__auto_type _a = (a); __auto_type _b = (b); (_b < _a) ? _b : _a;})

#define T(n) (n*(n + 1)/2)

int64_t **makeTriangle(size_t size){
	int64_t *_t = malloc(T(size)*sizeof(int64_t));
	if(!_t){
		return NULL;
	}
	int64_t **t = malloc(size*sizeof(int64_t*));
	if(!t){
		free(_t);
		return NULL;
	}
	for(int64_t i = 0; i < size; ++i){
		t[i] = _t + T(i);
	}
	return t;
}

int main(void){
	int64_t **a = makeTriangle(SIDE);
	if(!a){
		return 1;
	}
	for(int64_t t = 0, k = 0; k < T(SIDE); ++k){
		t = (615949*t + 797807)%(1<<20);
		(*a)[k] = t - (1<<19);
	}
	int64_t min = 0;
	for(int64_t i = 0; i < SIDE; ++i){
		#pragma omp parallel for reduction(min:min)
		for(int64_t j = 0; j <= i; ++j){
			min = MIN(min, a[i][j]);
			for(int64_t k = 1; i + k < SIDE; ++k){
				for(int64_t l = 0; l <= k; ++l){
					a[i][j] += a[i + k][j + l];
				}
				min = MIN(min, a[i][j]);
			}
		}
	}
	printf("%"PRId64"\n", min);
}

