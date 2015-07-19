#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sigmaSieve.h"

void mark(int *sigma, int m, int d){//divisor sums, multiple, divisor
	sigma[m] += d + m/d;
}

int *sigmaSieve(int max){
	int *sigma = malloc((max + 1)*sizeof(int));
	if(!sigma){
		return NULL;
	}
	sigma[1] = 0;
	for(int i = 2; i <= max; ++i){
		sigma[i] = 1;
	}
	for(int d = 2; d <= sqrt(max); ++d){//divisor
		sigma[d*d] += d;
		for(int m = d*(d + 1); m <= max; m += d){//multiple
			mark(sigma, m, d);
		}
	}
	return sigma;
}

