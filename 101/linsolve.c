#include <stdlib.h>
#include <gmp.h>
#include "linsolve.h"

int linsolve(size_t nxs, mpq_t **eqns, mpq_t *xs){
	mpq_t prod;
	mpq_init(prod);
	for(size_t c = 0; c < nxs; ++c){
		if(!mpz_cmp_ui(mpq_numref(eqns[c][c]), 0)){//this row starts with zero; try switching it with one that does not
			mpq_t *t;
			for(size_t i = c + 1; i < nxs; ++i){
				if(mpz_cmp_ui(mpq_numref(eqns[i][c]), 0)){
					t = eqns[i];
					eqns[i] = eqns[c];
					eqns[c] = t;
					goto SWITCHED_ROWS;
				}
			}
			mpq_clear(prod);
			return 0;
			SWITCHED_ROWS:;
		}
		for(size_t j = c; j <= nxs; ++j){//for each element
			//divide eqns[c][j] by eqns[c][c]
			mpq_div(eqns[c][j], eqns[c][j], eqns[c][c]);
		}
		for(size_t i = 0; i < nxs; ++i){//for each row
			if(i == c){
				continue;
			}
			for(size_t j = c; j <= nxs; ++j){//for each element
				//subtract eqns[c][j]*eqns[i][c] from eqns[i][j]
				mpq_mul(prod, eqns[c][j], eqns[i][c]);
				mpq_sub(eqns[i][j], eqns[i][j], prod);
			}
		}
	}
	mpq_clear(prod);
	for(size_t i = 0; i < nxs; ++i){
		mpq_set(xs[i], eqns[i][nxs]);
	}
	return 1;
}

