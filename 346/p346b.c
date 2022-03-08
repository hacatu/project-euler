#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "hash.h"

#define MAX 1000000000000ULL

inline static uint64_t u64_hash(const void *a){
	return *(const uint64_t*)a;
}

inline static int u64_cmp(const void *_a, const void *_b){
	uint64_t a = *(const uint64_t*)_a, b = *(const uint64_t*)_b;
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}
	return 0;
}

hashtbl_ft u64_counter_ft = {
	.size= sizeof(uint64_t),
	.hash= u64_hash,
	.cmp= u64_cmp,
	.load_factor= 0.7,
};

int main(void){
	hashtbl_t repunits;
	if(!hash_init(&repunits, &u64_counter_ft, 0)){
		printf("\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		return 1;
	}
	uint64_t s = 1;
	if(MAX > 31){
		s -= 31;
		if(MAX > 8191){
			s -= 8191;
		}
	}
	for(uint64_t b = 2;; ++b){
		uint64_t n = b*b + b + 1;
		if(n >= MAX){
			break;
		}
		for(; n < MAX; n = n*b + 1){
			//int status;
			//hash_insert(&repunits, &u64_counter_ft, &n, &status);
			//if(status == 0){
				// TODO: handle error
			//}else if(status == 1){
				s += n;
			//}
		}
	}
	hash_destroy(&repunits, &u64_counter_ft);
	printf("%"PRIu64"\n", s);
}