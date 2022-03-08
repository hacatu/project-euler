#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>

#define D 5
#define N 10
#define MAX_R (N/10)
#define MAX_B (N/5)
#define MAX_L MAX_B

uint64_t *_ways;

ptrdiff_t l_idx_stride[D + 1] = {
	1, MAX_L + 1, (MAX_L + 1)*(MAX_L + 1), (MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1), (MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1), (MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1)*(MAX_L + 1)
}, r_idx_stride[D + 1] = {
	1, MAX_R + 1, (MAX_R + 1)*(MAX_R + 1), (MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1), (MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1), (MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1)*(MAX_R + 1)
};

uint64_t *_ways_ent(uint_fast8_t l_counts[static D], uint_fast8_t r_counts[static D], uint_fast8_t f){
	uint64_t *ent = _ways + f*l_idx_stride[D]*r_idx_stride[D];
	for(size_t i = 0; i < D; ++i){
		ent += l_idx_stride[D]*r_idx_stride[i]*r_counts[i];
		ent += l_idx_stride[i]*l_counts[i];
	}
	return ent;
}

uint64_t ways(uint_fast8_t l_counts[static D], uint_fast8_t r_counts[static D], uint_fast8_t f){
	uint64_t *ent = _ways_ent(l_counts, r_counts, f);
	if(*ent == UINT64_MAX){
		*ent = 0;
		if(l_counts[f]){
			--l_counts[f];
			*ent += ways(l_counts, r_counts, (f + 1)%D);
			++l_counts[f];
		}
		if(r_counts[f]){
			--r_counts[f];
			*ent += ways(l_counts, r_counts, (f + D - 1)%D);
			++r_counts[f];
		}
	}
	return *ent;
}

int main(){
	size_t _ways_size = D*l_idx_stride[D]*r_idx_stride[D]*sizeof(uint64_t);
	_ways = malloc(_ways_size);
	if(!_ways){
		printf("Could not yoink computer's whole RAM\n");
		exit(1);
	}
	printf("Allocated %zu yummy bytes\n", _ways_size);
	uint_fast8_t l_counts[D] = {}, r_counts[D] = {};
	for(size_t i = 0; i < D; ++i){
		*_ways_ent(l_counts, r_counts, i) = 1;
	}
	for(uint64_t b = N%2; b <= MAX_B; b += 2){
		uint64_t R = (N - 5*b)/10;
		for(size_t i = 0; i < D; ++i){
			l_counts[i] = R + b;
			r_counts[i] = R;
		}
		uint64_t btw = ways(l_counts, r_counts, 0);
		printf("N=%"PRIu64", b=%"PRIu64": ways=%"PRIu64"\n", (uint64_t)N, b, btw);
	}
}

