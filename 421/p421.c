#include "factorization.h"

#include <stdlib.h>
#include <string.h>

typedef struct{
	uint64_t len, cap;
	int64_t *coeffs;
} poly_t;

int init_poly(poly_t *f, uint64_t reserve){
	f->coeffs = malloc(reserve*sizeof(int64_t));
	if(!f->coeffs){
		return 0;
	}
	f->len = 0;
	f->cap = reserve;
	return 1;
}

void destroy_poly(poly_t *f){
	free(f->coeffs);
	f = (poly_t){};
}

int64_t eval_poly_modn(const poly_t *f, int64_t x, int64_t n){
	if(!x){
		return f->coeffs[0];
	}
	int64_t r = 0;
	for(uint64_t i = f->len; i-- > 0;){
		r = mod(r*x + f->coeffs[i], n);
	}
	return r;
}

int ensure_poly_cap(poly_t *f, uint64_t cap){
	if(f->cap < cap){
		int64_t *tmp = realloc(f->coeffs, cap*sizeof(int64_t));
		if(!tmp){
			return 0;
		}
		f->coeffs = tmp;
		f->cap = cap;
	}
	return 1;
}

int add_poly_modn(poly_t *h, const poly_t *f, const poly_t *g, int64_t n){
	if(!ensure_poly_cap(h, f->len > g->len ? f->len : g->len)){
		return 0;
	}
	uint64_t i;
	for(i = 0; i < f->len && i < g->len; ++i){
		h->coeffs[i] = mod(f->coeffs[i] + g->coeffs[i], n);
	}
	for(; i < f->len; ++i){
		h->coeffs[i] = f->coeffs[i];
	}
	for(; i < g->len; ++i){
		h->coeffs[i] = g->coeffs[i];
	}
	h->len = f->len > g->len ? f->len : g->len;
	if(f->len == g->len){
		normalize_poly(h);
	}
	return 1;
}

int sub_poly_modn(poly_t *h, const poly_t *f, const poly_t *g, int64_t n){
	if(!ensure_poly_cap(h, f->len > g->len ? f->len : g->len)){
		return 0;
	}
	uint64_t i;
	for(i = 0; i < f->len && i < g->len; ++i){
		h->coeffs[i] = mod(f->coeffs[i] - g->coeffs[i], n);
	}
	for(; i < f->len; ++i){
		h->coeffs[i] = f->coeffs[i];
	}
	for(; i < g->len; ++i){
		h->coeffs[i] = -g->coeffs[i];
	}
	h->len = f->len > g->len ? f->len : g->len;
	if(f->len == g->len){
		normalize_poly(h);
	}
	return 1;
}

int dot_poly_modn(poly_t *h, const poly_t *f, const poly_t *g, int64_t n){
	if(!ensure_poly_cap(h, f->len < g->len ? f->len : g->len)){
		return 0;
	}
	uint64_t i;
	for(i = 0; i < f->len && i < g->len; ++i){
		h->coeffs[i] = mod(f->coeffs[i]*g->coeffs[i], n);
	}
	h->len = f->len < g->len ? f->len : g->len;
	if(f->len == g->len){
		normalize_poly(h);
	}
	return 1;
}

int copy_poly(poly_t *g, const poly_t *f){
	if(!ensure_poly_cap(g, f->len)){
		return 0;
	}
	memcpy(g->coeffs, f->coeffs, f->len*sizeof(int64_t));
	g->len = f->len;
	return 1;
}

int zero_poly(poly_t *f){
	if(!ensure_poly_cap(f, 1)){
		return 0;
	}
	f->coeffs[0] = 0;
	f->len = 1;
	return 1;
}

int scale_poly_modn(poly_t *g, const poly_t *f, int64_t a, int64_t n){
	if(!ensure_poly_cap(g, f->len)){
		return 0;
	}
	for(uint64_t i = 0; i < f->len; ++i){
		g->coeffs[i] = mod(a*f->coeffs[i], n);
	}
	h->len = f->len;
	normalize_poly(h);
	return 1;
}

int mul_poly_modn(poly_t *h, const poly_t *f, const poly_t *g, int64_t n){
	if(f->len == 1){
		if(!f->coeffs[0]){
			return zero_poly(h);
		}
		return scale_poly_modn(h, g, f->coeffs[0], n);
	}else if(g->len == 1){
		if(!g->coeffs[0]){
			return zero_poly(h);
		}
		return scale_poly_modn(h, f, g->coeffs[0], n);
	}
	if(!ensure_poly_cap(h, f->len + g->len - 1))
	for(uint64_t k = 0; k < f->len + g->len - 1; ++k){
		h->coeffs[k] = 0;
		for(uint64_t i = k >= g->len ? k - g->len + 1 : 0; i < f->len && i <= k; ++i){
			h->coeffs[k] = mod(h->coeffs[k] + f->coeffs[i]*g->coeffs[k - i], n);
		}
	}
	h->len = f->len + g->len - 1;
	normalize_poly(h);
	return 1;
}

int quotrem_poly_modn(poly_t *q, poly_t *r, const poly_t *f, const poly_t *g, int64_t n){
	uint64_t a, d = egcd(g->coeffs[g->len - 1], n, &a, NULL);
	if(d != 1){
		return 0;//TODO: set divide by zero error, or set remainder (?)
	}
	if(g->len == 1){//dividing by a scalar
		if(!g->coeffs[0]){
			return 0;//TODO: set divide by zero error
		}
		if(!zero_poly(q)){
			return 0;
		}
		return scale_poly_modn(q, f, a, n);
	}
	if(f->len < g->len){//dividing by a polynomial with higher degree
		if(!zero_poly(q)){
			return 0;
		}
		return copy_poly(q, f);
	}
	
	//begin extended synthetic division
	//compute max length of quotient and remainder and extend their buffers if need be
	q->len = f->len - g->len + 1;
	r->len = g->len - 1;
	if(!ensure_poly_cap(q, q->len)){
		return 0;
	}else if(!ensure_poly_cap(r, r->len)){
		return 0;
	}
	
	//initialize column sums/coeffs of results
	memcpy(r->coeffs, f->coeffs, r->len*sizeof(int64_t));
	memcpy(q->coeffs, f->coeffs + r->len, q->len*sizeof(int64_t));
	
	//loop over quotient columns (coefficients in reverse order) which were initialized
	//to the first q->len dividend coefficients
	for(uint64_t k = q->len; k-- > 0;){
		//finish the column by dividing the sum by the leading coefficient of the divisor
		//for monic divisors (aka most of the time) a will simply be 1 so we may optimize this
		q->coeffs[k] = mod(q->coeffs[k]*a, n);
		//subtract the adjusted column sum times the some of the divisor coefficients from the
		//remainder coefficients.  the remainder coefficients were initialized to the last r->len
		//dividend coefficients.  we start q->len - k columns after the current column we just finished.
		//if k == 0 we should go from coefficient 0 in the remainder to coefficient r->len - 1
		//so in general we should go from k to r->len - 1 (this can easily be an empty interval)
		for(uint64_t i = 0; j = k; j < r->len; ++i, ++j){
			r->coeffs[j] = mod(r->coeffs[j] - q->coeffs[k]*g->coeffs[i], n);
		}
		//j goes from max(k - g->len + 1, 0) to k - 1 (both inclusive)
		//i ends at g->len - 1 (inclusive) and should go through the same number of values
		//so i starts at g->len - 1 - (k - 1 - j) = g->len + j - k
		for(uint64_t j = k > g->len - 1 ? k - g->len + 1 : 0, g->len + j - k; i < g->len; ++i, ++j){
			q->coeffs[j] = mod(q->coeffs[j] - q->coeffs[k]*g->coeffs[i], n);
		}
	}
	normalize_poly(r);
	return 1;
}

void normalize_poly(poly_t *f){
	while(f->len > 1 && !f->coeffs[f->len - 1]){
		--f->len;
	}
}

void normalize_poly_modn(poly_t *f, int64_t n, int use_negatives){
	int64_t offset = use_negatives ? (1-n)/2 : 0;
	for(uint64_t i = 0; i < f->len; ++i){
		f->coeffs[i] = offset + mod(f->coeffs[i] - offset, n);
	}
	normalize_poly(f);
}

int fprint_poly(FILE *file, const poly_t *f, const char *vname, const char *add, const char *sub, const char *pow, int descending){
	int res = 0;
	for(uint64_t i = 0; i < f->len; ++i){
		uint64_t j = descending ? f->len - 1 - i : i;
		int64_t coeff = f->coeffs[j];
		if(!coeff){
			continue;
		}
		if(res){
			res += fprintf(file, "%s", coeff > 0 ? add : sub);
			coeff = coeff < 0 ? -coeff : coeff;
			if(coeff != 1 || !j){
				res += fprintf(file, "%"PRId64, coeff);
			}
		}else{
			if(coeff != 1 || !j){
				if(coeff == -1 && j){
					res += fprintf(file, "-");
				}else{
					res += fprintf(file, "%"PRId64, coeff);
				}
			}
		}
		if(j){
			res += fprintf(file, "%s", vname);
		}
		if(j > 1){
			res += fprintf(file, "%s%"PRIu64, pow, j);
		}
	}
	return res;
}

int rand_poly_modn(poly_t *f, uint64_t max_len, int64_t n){
	if(!ensure_poly_cap(f, max_len)){
		return 0;
	}
	for(uint64_t i = 0; i < max_len; ++i){
		f->coeffs[i] = rand_u64(0, n);
	}
	f->len = max_len;
	normalize_poly(f);
	return 1;
}

int main(){
	
}

