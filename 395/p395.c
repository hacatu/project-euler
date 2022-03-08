#define _GNU_SOURCE
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#define AT_RBUF_GROWTH_FACTOR 8

typedef struct{
	complex double a;
	complex double c;
} affine_transform;

typedef struct{
	double bottom, top, left, right, tolerance, a_min, a_max;
} tree_bounds;

typedef struct{
	size_t a, b, len, cap;
	affine_transform *buf;
} AT_ring_buf;

static const affine_transform gens[2] = {{
	.a=16./25 + 12*I/25,
	.c=I
},
{
	.a=9./25 - 12*I/25,
	.c=16./25 + 37*I/25
}
};

static const complex double root = 16./25 + 37*I/25;

static inline void update_bounds(tree_bounds *bounds, complex double z){
	if(creal(z) < bounds->left){
		bounds->left = creal(z);
	}else if(creal(z) > bounds->right){
		bounds->right = creal(z);
	}
	if(cimag(z) < bounds->bottom){
		bounds->bottom = cimag(z);
	}else if(cimag(z) > bounds->top){
		bounds->top = cimag(z);
	}
}

/*
 * 28.2453731281157481
 * 28.2453731281157516
 * 28.2453731281157587
 */

static inline int check_bounds(const tree_bounds *bounds, complex double z){
	return
		creal(z) - bounds->left < bounds->tolerance ||
		bounds->right - creal(z) < bounds->tolerance ||
		cimag(z) - bounds->bottom < bounds->tolerance ||
		bounds->top - cimag(z) < bounds->tolerance;
}

static int update(tree_bounds *bounds, AT_ring_buf *ts){
	bounds->tolerance *= 0.8;
	size_t next_cap = ts->cap;
	if(ts->cap < ts->len*2){
		printf("Reallocating AT rbuf\n");
		next_cap *= AT_RBUF_GROWTH_FACTOR;
		affine_transform *tbuf = realloc(ts->buf, next_cap*sizeof(affine_transform));
		if(!tbuf){
			printf("Ran out of memory!\n");
			return 0;
		}
		ts->buf = tbuf;
	}
	size_t next_a = next_cap == ts->cap ? ts->b : ts->cap;
	size_t next_len = 0;
	size_t j = next_a;
	for(size_t i = ts->a;;){
		complex double a = ts->buf[i].a, c = ts->buf[i].c;
		
		for(size_t w = 0; w < 2; ++w){
			complex double a_ = gens[w].a*a, c_ = gens[w].c*a + c;
			complex double z = a_*root + c_;
			update_bounds(bounds, z);
			if(check_bounds(bounds, z)){
				ts->buf[j] = (affine_transform){.a=a_, .c=c_};
				j = (j + 1)%next_cap;
				++next_len;
			}
		}
		
		i = (i + 1)%ts->cap;
		if(i == ts->b){
			break;
		}
	}
	ts->cap = next_cap;
	ts->len = next_len;
	ts->a = next_a;
	ts->b = j;
	bounds->a_min = (bounds->top - bounds->bottom)*(bounds->right - bounds->left);
	bounds->a_max = (bounds->top - bounds->bottom + 2*bounds->tolerance)*(bounds->right - bounds->left + 2*bounds->tolerance);
	printf("%zu points near boundary\n", ts->len);
	printf("lo area: %.16f\nhi area: %.16f\n", bounds->a_min, bounds->a_max);
	return 1;
}

int main(){
	tree_bounds bounds = {.top=1, .right=1, .tolerance=5*M_SQRT2};
	AT_ring_buf ts = {.b=1, .len=1, .cap=1000000, .buf=malloc(1000000*sizeof(affine_transform))};
	if(!ts.buf){
		printf("Could not allocate AT rbuf\n");
		exit(EXIT_FAILURE);
	}
	ts.buf[0] = (affine_transform){.a=1};
	do{
		if(!update(&bounds, &ts)){
			exit(EXIT_FAILURE);
		}
	}while(floor(bounds.a_min*1e11) != floor(bounds.a_max*1e11));
	free(ts.buf);
}

