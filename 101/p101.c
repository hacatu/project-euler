#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <alloca.h>
#include "linsolve.h"

#define MAX_POINTS 10

void nInit(size_t n, mpq_t *x){
	for(size_t i = 0; i < n; ++i){
		mpq_init(x[i]);
	}
}

void nClear(size_t n, mpq_t *x){
	for(size_t i = 0; i < n; ++i){
		mpq_clear(x[i]);
	}
}

void malloc2dInit(size_t n, size_t m, mpq_t **a){
	for(size_t i = 0; i < n; ++i){
		a[i] = malloc(m*sizeof(mpq_t));
		nInit(m, a[i]);
	}
}

void copy2d(size_t n, size_t m, mpq_t **dest, mpq_t **src){
	for(size_t i = 0; i < n; ++i){
		for(size_t j = 0; j < m; ++j){
			mpq_set(dest[i][j], src[i][j]);
		}
	}
}

void free2dClear(size_t n, size_t m, mpq_t *a[n]){
	for(size_t i = 0; i < n; ++i){
		nClear(m, a[i]);
		free(a[i]);
	}
}

void pnEval(mpq_t y, size_t n, int x, mpq_t *xs, mpq_t **points){//must pass in initialized points array
	mpq_t prod;
	mpq_init(prod);
	mpq_set_si(y, 0, 1);
	for(size_t i = 0; i < n; ++i){
		mpq_mul(prod, xs[i], points[x - 1][i]);
		mpq_add(y, y, prod);
	}
	mpq_clear(prod);
}

void copyYs(size_t n, size_t m, mpq_t **a, mpq_t *ys){
	for(size_t i = 0; i < n; ++i){
		mpq_set(a[i][m], ys[i]);
	}
}

int main(void){
	mpq_t *points[MAX_POINTS] = {};
	mpq_t *eqns[MAX_POINTS] = {};
	mpq_t ns[MAX_POINTS];
	mpq_t xs[MAX_POINTS];
	mpq_t ys[MAX_POINTS];
	mpq_t sfs;
	mpq_t term;
	mpq_init(sfs);
	mpq_init(term);
	nInit(MAX_POINTS, ns);
	nInit(MAX_POINTS, xs);
	nInit(MAX_POINTS, ys);
	malloc2dInit(MAX_POINTS, MAX_POINTS + 1, points);
	malloc2dInit(MAX_POINTS, MAX_POINTS + 1, eqns);
	for(int i = 0, a = 1; i < MAX_POINTS; ++i, a *= -1){
		mpq_set_si(ns[i], a, 1);
	}
	for(int x = 1; x <= MAX_POINTS; ++x){//fill the base array with powers of every number from 1 to 10
		for(int j = 0, p = 1; j < MAX_POINTS; ++j, p *= x){
			mpq_set_si(points[x - 1][j], p, 1);
		}
	}
	for(int x = 1; x <= MAX_POINTS; ++x){//calculate the 10 true points
		pnEval(ys[x - 1], MAX_POINTS, x, ns, points);
		mpq_set(points[x - 1][MAX_POINTS], ys[x - 1]);
	}
	int max;
	for(size_t i = 0; i < MAX_POINTS; ++i){
		copy2d(i + 1, i + 2, eqns, points);
		copyYs(i + 1, i + 2, eqns, ys);
		if(!linsolve(i + 1, eqns, xs)){
			puts("Failed to solve");
			goto CLEANUP;
		}
		max = i + 2;
		while((pnEval(term, i + 1, max, xs, points), !mpq_equal(term, ys[max]))){
			++max;
		}
		mpq_add(sfs, sfs, term);
	}
	mpq_out_str(stdout, 10, sfs);
	CLEANUP:
	nClear(MAX_POINTS, ns);
	nClear(MAX_POINTS, xs);
	nClear(MAX_POINTS, ys);
	free2dClear(MAX_POINTS, MAX_POINTS + 1, points);
	free2dClear(MAX_POINTS, MAX_POINTS + 1, eqns);
}

