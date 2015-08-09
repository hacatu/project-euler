#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <inttypes.h>
#include <hac_util.h>
#include <hac_vec.h>

#define MAX 120000

int isSquare(uint64_t n){
	uint64_t r;
	switch(n&0x3F){
		case 0x00: case 0x01: case 0x04: case 0x09: case 0x10: case 0x11:
		case 0x19: case 0x21: case 0x24: case 0x29: case 0x31: case 0x39:
		r = sqrt(n);
		return n == r*r;
		default:
		return 0;
	}
}

int compUint64_ts(uint64_t e1, uint64_t e2){
	if(e1 < e2){
		return -1;
	}else if(e1 > e2){
		return 1;
	}
	return 0;
}

int main(void){
	HAC_VEC_T_NEW(uint64_t);
	
	HAC_VEC_T(uint64_t) sides[MAX + 1 - 2 - 3];
	int sums[MAX + 1] = {0};
	uint64_t s = 0;
	
	for(uint64_t p = 2; p < MAX + 1 - 2 - 3; ++p){
		sides[p] = HAC_VEC_EMPTY(uint64_t);
	}
	
	#pragma omp parallel for
	for(uint64_t p = 2; p < MAX + 1 - 2 - 3; ++p){
		for(uint64_t q = 2; q < MAX + 1 - p - 2; ++q){
			if(isSquare(p*p + p*q + q*q)){
				#pragma omp critical
				{
					HAC_VEC_PUSHR(uint64_t, &sides[p], q);
				}
			}
		}
	}
	
	#pragma omp parallel for
	for(uint64_t p = 2; p < MAX + 1 - 2 - 3; ++p){
		for(uint64_t i = 0, q; i < sides[p].n; ++i){
			q = sides[p].a[i];
			if(p + q + 2 > MAX){
				break;
			}
			for(uint64_t i = 0, r, l; i < sides[q].n; ++i){
				r = sides[q].a[i];
				l = p + q + r;
				if(l > MAX){
					break;
				}
				if(!sums[l]){
					if(HAC_VEC_CONTAINS_S(uint64_t, &sides[r], p, compUint64_ts(e1, e2))){
						#pragma omp critical
						{
							sums[l] = 1;
							s += l;
						}
					}
				}
			}
		}
	}
	
	printf("%"PRIu64"\n", s);
	
	for(uint64_t p = 2; p < MAX + 1 - 2 - 3; ++p){
		HAC_VEC_DELETE(uint64_t, &sides[p]);
	}
}

