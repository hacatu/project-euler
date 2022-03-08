#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

double A(uint64_t t, uint64_t g){
	if(!t){
		return pow(2, g);
	}else if(!g){
		return 1;
	}
	double *Ainv = calloc((t + 1)*(g + 1), sizeof(double));
	if(!Ainv){
		abort();
	}
	for(uint64_t i = 0; i <= t; ++i){
		Ainv[i*(g + 1)] = 1;
	}
	for(uint64_t i = 1; i <= g; ++i){
		Ainv[i] = pow(.5, i);
	}
	for(uint64_t i = 2; i <= t + g; ++i){
		for(uint64_t j = 1; j + 1 <= i && j <= t; ++j){
			Ainv[j*(g + 1) + i - j] = (Ainv[(j - 1)*(g + 1) + i - j] + Ainv[j*(g + 1) + i - j - 1])/2;
		}
	}
	double res = 1/Ainv[t*(g + 1) + g];
	free(Ainv);
	return res;
}

int main(int argc, char **argv){
	if(argc != 2){
		fprintf(stderr, "\e[1;31mInvalid usage, please specify 'n'\e[0m\n");
		exit(EXIT_FAILURE);
	}
	char *end;
	uint64_t n = strtoull(argv[1], &end, 10);
	if(end == argv[1]){
		fprintf(stderr, "\e[1;31mInvalid value for 'n'\e[0m\n");
		exit(EXIT_FAILURE);
	}
	printf("A(%"PRIu64")=%f\n", n, A(n, n));
}

