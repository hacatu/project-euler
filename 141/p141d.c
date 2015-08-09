#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>
#include <stdatomic.h>

#define MAX 1000000000000

uint64_t s = 0;

typedef struct{
	uint64_t start, step, *fsms, max;
} doRangeArg;

int compUint64_ts(uint64_t a, uint64_t b){
	if(a > b){
		return -1;
	}else if(a < b){
		return 1;
	}
	return 0;
}

void mark(uint64_t *fsms, uint64_t p, uint64_t m){
	uint64_t n = m, r = 0;
	while(!(n%p)){//add 1 to the power on the prime p until m is not divisble by p
		n /= p;
		++r;
	}
	r = (r + 1)/2;
	while(r){
		fsms[m] *= p;
		--r;
	}
}

void *doRange(void *vp){
	doRangeArg arg = *(doRangeArg*)vp;
	for(uint64_t a = arg.start; a < arg.max; a += arg.step){
		for(uint64_t b = (a/arg.fsms[a] + 1)*arg.fsms[a], n, r;; b += arg.fsms[a]){
			n = b*b*b/a + a;
			if(n >= MAX){
				break;
			}
			r = sqrtl(n);
			if(n == r*r){
				//printf("%"PRIu64"\n", n);
				atomic_fetch_add(&s, n);
			}
		}
	}
	return NULL;
}

int main(void){
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 65536);
	
	uint64_t cpus = 6;
	
	pthread_t threads[cpus];
	doRangeArg arg[cpus];
	
	uint64_t max = sqrtl(MAX);
	uint64_t *fsms = malloc(max*sizeof(uint64_t));
	for(uint64_t i = 0; i < max; ++i){
		fsms[i] = 1;
	}
	for(uint64_t p = 2; p < max; ++p){
		if(fsms[p] != 1){//p is not prime
			continue;
		}//p is prime, mark off its multiples
		for(uint64_t m = p; m < max; m += p){
			mark(fsms, p, m);
		}
	}
	
	for(uint64_t i = 0; i < cpus; ++i){
		arg[i] = (doRangeArg){.start = i + 1, .step = cpus, .fsms = fsms, .max = max};
		if(pthread_create(&threads[i], &attr, doRange, &arg[i])){
			return 1;
		}
	}
	for(uint64_t i = 0; i < cpus; ++i){
		if(pthread_join(threads[i], NULL)){
			return 1;
		}
	}
	
	free(fsms);
	printf("%"PRIu64"\n", s);
}

