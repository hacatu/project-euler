#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

inline static uint64_t maximize_grid(const uint64_t n, const uint64_t mat[static n*n]){
	uint64_t *memo = calloc(sizeof(uint64_t), (n + 1)*(1ULL << n));
	if(!memo){
		// TODO: handle error
	}
	for(uint64_t y = 0; y < n; ++y){
		for(uint64_t x_bits = 0; x_bits < (1ULL << n); ++x_bits){
			for(uint64_t x = 0; x < n; ++x){
				if((1ULL << x) & x_bits){
					continue;
				}
				uint64_t i = (y + 1)*(1ULL << n) + (x_bits | (1ULL << x));
				uint64_t m = memo[i];
				uint64_t v = memo[y*(1ULL << n) + x_bits] + mat[y*n + x];
				if(v > m){
					memo[i] = v;
				}
			}
		}
	}
	uint64_t m = memo[n*(1ULL << n) + ~(UINT64_MAX << n)];
	free(memo);
	return m;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("\e[1;31mERROR: Invalid invocation.  Please use like:\n%s <file>\e[0m\n", argv[0]);
		goto FAILED_OPEN_FILE;
	}
	FILE *file = fopen(argv[1], "r");
	if(!file){
		printf("\e[1;31mERROR: Could not open file \"%s\": %s\e[0m\n", argv[1], strerror(errno));
		goto FAILED_OPEN_FILE;
	}
	char *line_buf = NULL;
	size_t line_len = 0;
	if(getline(&line_buf, &line_len, file) < 2){
		printf("\e[1;31mERROR: Could not get first line: %s\e[0m\n", strerror(errno));
		goto FAILED_GET_FIRST_LINE;
	}
	size_t n = 0;
	uint64_t *mat = malloc(62*sizeof(uint64_t));
	if(!mat){
		printf("\e[1;31mERROR: Could not allocate first row: %s\e[0m\n", strerror(errno));
		goto FAILED_ALLOC_MAT;
	}
	const char *line_tail = line_buf;
	while(1){
		char *d_end;
		uint64_t d = strtoull(line_tail, &d_end, 10);
		if(d_end == line_tail){
			printf("\e[1;31mERROR: Input file formatted wrong at %s:1:%td\nstrtoull error: %s\e[0m\n", argv[1], line_tail - line_buf, strerror(errno));
			goto FAILED_PARSE;
		}
		mat[n] = d;
		++n;
		if(*d_end == '\n' || *d_end == '\0'){
			break;
		}else if(n == 63){
			printf("\e[1;31mERROR: Matrix is wider than supported (max 62)!\e[0m\n");
			goto FAILED_PARSE;
		}
		line_tail = d_end;
	}
	void *tmp = realloc(mat, n*n*sizeof(uint64_t));
	if(!tmp){
		printf("\e[1;31mERROR: Could not allocate matrix: %s\e[0m\n", strerror(errno));
		goto FAILED_PARSE;
	}
	mat = tmp;
	for(uint64_t y = 1; y < n; ++y){
		if(getline(&line_buf, &line_len, file) < 2){
			printf("\e[1;31mERROR: Could not get line %"PRIu64": %s\e[0m\n", y, strerror(errno));
			goto FAILED_PARSE;
		}
		line_tail = line_buf;
		for(uint64_t x = 0; x < n; ++x){
			char *d_end;
			uint64_t d = strtoull(line_tail, &d_end, 10);
			if(d_end == line_tail){
				printf("\e[1;31mERROR: Input file formatted wrong at %s:1:%td\nstrtoull error: %s\e[0m\n", argv[1], line_tail - line_buf, strerror(errno));
				goto FAILED_PARSE;
			}
			mat[y*n + x] = d;
			line_tail = d_end;
		}
	}
	free(line_buf);
	fclose(file);
	uint64_t m = maximize_grid(n, mat);
	free(mat);
	printf("matrix sum: %"PRIu64"\n", m);
	return 0;
	FAILED_PARSE:;
	free(mat);
	FAILED_ALLOC_MAT:;
	free(line_buf);
	FAILED_GET_FIRST_LINE:;
	fclose(file);
	FAILED_OPEN_FILE:;
	return 1;
}

