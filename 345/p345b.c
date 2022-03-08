#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include "hash.h"

#define LOAD_FACTOR 0.7

typedef struct{
	uint64_t x_bits, y_bits, max;
} submatrix;

/*
 * Memoization: for an n by n grid, we need to remember all subgrids
 * of size 2 by 2 to size n - 1 by n - 1.  How many k by k grids
 * are there?  (n choose k)^2.  So we are interested in
 * (n 2)^2 + (n 3)^2 + ... + (n n - 1)^2 = (2n n) - n^2 - 2
 * ~= (2n)!/(n!)^2
 * ~= sqrt(2pi(2n))(2n/e)^(2n)/(sqrt(2pi*n)(n/e)^n)^2
 * = sqrt(4pi*n)/(2pi*n) * 4^n*n^(2n)/n^(2n) * e^(2n)/e^(2n)
 * = sqrt(pi*n)/(pi*n) * 4^n
 * = 4^n/sqrt(pi*n)
 */

uint64_t hash_submatrix(const void *_o){
	const submatrix *o = _o;
	return o->x_bits + o->y_bits*2147483629UL;
}

int cmp_submatrix(const void *_a, const void *_b){
	const submatrix *a = _a, *b = _b;
	if(a->y_bits < b->y_bits){
		return -1;
	}else if(a->y_bits > b->y_bits){
		return 1;
	}else if(a->x_bits < b->x_bits){
		return -1;
	}else if(a->x_bits > b->x_bits){
		return 1;
	}
	return 0;
}

hashtbl_ft submatrix_ft = {
	.size= sizeof(submatrix),
	.hash= hash_submatrix,
	.cmp= cmp_submatrix,
	.load_factor= LOAD_FACTOR,
};

inline static uint64_t maximize_k_grid(uint64_t n, uint64_t k, const uint64_t mat[static n*n], hashtbl_t *k_1_sums, uint64_t x_bits, uint64_t y_bits){
	uint64_t m = 0;
	uint64_t y_shift = __builtin_ctzll(y_bits);
	for(uint64_t x_shift = 0; (UINT64_MAX << x_shift) & x_bits; ++x_shift){
		if(!(((uint64_t)1 << x_shift) & x_bits)){
			continue;
		}
		submatrix *s = hash_get(k_1_sums, &submatrix_ft, &(submatrix){x_bits^((uint64_t)1 << x_shift), y_bits^((uint64_t)1 << y_shift)});
		//if s is null we're wrecked anyway
		uint64_t v = mat[y_shift*n + x_shift] + s->max;
		if(v > m){
			m = v;
		}
	}
	return m;
}

inline static void init_2_grid(uint64_t n, const uint64_t mat[static n*n], hashtbl_t *_2_sums){
	for(uint64_t y_a = 1; y_a < n; ++y_a){
		for(uint64_t x_a = 1; x_a < n; ++x_a){
			for(uint64_t y_b = 0; y_b < y_a; ++y_b){
				for(uint64_t x_b = 0; x_b < x_a; ++x_b){
					uint64_t x_bits = ((uint64_t)1 << x_a) | ((uint64_t)1 << x_b);
					uint64_t y_bits = ((uint64_t)1 << y_a) | ((uint64_t)1 << y_b);
					uint64_t major = mat[y_a*n + x_a] + mat[y_b*n + x_b];
					uint64_t minor = mat[y_a*n + x_b] + mat[y_b*n + x_a];
					if(!hash_insert(_2_sums, &submatrix_ft, &(submatrix){x_bits, y_bits, major < minor ? minor : major}, NULL)){
						printf("\e[1;31mERROR: %s: could not insert element into hash table\e[0m\n", __func__);
						exit(1);
					}
				}
			}
		}
	}
}

inline static void step_k_grid(uint64_t n, uint64_t k, const uint64_t mat[static n*n], hashtbl_t *k_1_sums, hashtbl_t *k_sums){
	for(uint64_t x = ~(UINT64_MAX << k); x < ((uint64_t)1 << n);){
		for(uint64_t y = ~(UINT64_MAX << k); y < ((uint64_t)1 << n);){
			uint64_t m = maximize_k_grid(n, k, mat, k_1_sums, x, y);
			if(!hash_insert(k_sums, &submatrix_ft, &(submatrix){x, y, m}, NULL)){
				printf("\e[1;31mERROR: %s: could not insert element into hash table\e[0m\n", __func__);
				exit(1);
			}
			uint64_t t = y | (y - 1);
			y = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(y) + 1));
		}
		uint64_t t = x | (x - 1);
		x = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(x) + 1));
	}
}

inline static uint64_t maximize_grid(uint64_t n, const uint64_t mat[static n*n]){
	hashtbl_t tbl_a, tbl_b;
	if(!hash_init(&tbl_a, &submatrix_ft, (2 << n)*M_1_PI/LOAD_FACTOR)){
		printf("\e[1;31mERROR: %s: could not initialize hash table\e[0m\n", __func__);
		exit(1);
	}
	if(!hash_init(&tbl_b, &submatrix_ft, (2 << n)*M_1_PI/LOAD_FACTOR)){
		printf("\e[1;31mERROR: %s: could not initialize hash table\e[0m\n", __func__);
		exit(1);
	}
	init_2_grid(n, mat, &tbl_a);
	uint64_t ret;
	for(uint64_t k = 3;; ++k){
		printf("k = %"PRIu64"\n", k);
		if(k == n){
			ret = maximize_k_grid(n, k, mat, &tbl_a, ~(UINT64_MAX << k), ~(UINT64_MAX << k));
			break;
		}
		step_k_grid(n, k, mat, &tbl_a, &tbl_b);
		hash_clear(&tbl_a, &submatrix_ft);
		++k;
		printf("k = %"PRIu64"\n", k);
		if(k == n){
			ret = maximize_k_grid(n, k, mat, &tbl_b, ~(UINT64_MAX << k), ~(UINT64_MAX << k));
			break;
		}
		step_k_grid(n, k, mat, &tbl_b, &tbl_a);
		hash_clear(&tbl_b, &submatrix_ft);
	}
	hash_destroy(&tbl_a, &submatrix_ft);
	hash_destroy(&tbl_b, &submatrix_ft);
	return ret;
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

