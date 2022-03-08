#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <hac_avl.h>

typedef struct{
    double p, q;
    uint64_t n;
} ratio_counter;

int cmp_ratios(const void *data, const void *_a, const void *_b){
    const ratio_counter *a = _a, *b = _b;
    double ord = a->p*b->q - b->p*a->q;
    if(ord < -8e-10){
        return -1;
    }else if(ord > 8e-10){
        return 1;
    }
    return 0;
}

int combine_ratio_counters(void *data, void *_a, void *_b){
    ratio_counter *a = _a, *b = _b;
    a->n += b->n;
    return 1;
}

avl_ft ratio_counter_avlft = {
    .size= sizeof(ratio_counter),
    .cmp= cmp_ratios,
    .add= combine_ratio_counters,
    
	void *(*alloc)(void *data);
	void (*free)(void *data, void*);
	void *data;
};


int main(void){
	
}

