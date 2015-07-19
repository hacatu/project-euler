#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000000
#define WHEEL_SIZE 30
#define ORTH_SIZE 8

#define OVERESTIMATE_PRIME_COUNT(n) ((size_t)(1.25506*n/log(n)))//this constant is described at oeis 209883

HAC_VEC_T_NEW(uint64_t);

const unsigned char offsets[ORTH_SIZE] = {6, 4, 2, 4, 2, 4, 6, 2};

const unsigned char lut_masks[WHEEL_SIZE] = {[1]=0x01,[7]=0x02,[11]=0x04,[13]=0x08,[17]=0x10,[19]=0x20,[23]=0x40,[29]=0x80};

int main(void){
	clock_t start = clock();
	char *is_composite = calloc(MAX/WHEEL_SIZE*ORTH_SIZE/8, sizeof(char));
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, OVERESTIMATE_PRIME_COUNT(MAX));
	HAC_VEC_PUSHR(uint64_t, &primes, 2);
	HAC_VEC_PUSHR(uint64_t, &primes, 3);
	HAC_VEC_PUSHR(uint64_t, &primes, 5);
	for(uint64_t n = 7, s1 = 1; n < MAX; n += offsets[s1], ++s1 == ORTH_SIZE ? s1 = 0 : 0){
		if(is_composite[n/WHEEL_SIZE]&lut_masks[n%WHEEL_SIZE]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		for(uint64_t m = n*n, s2 = s1; m < MAX; m += offsets[s2]*n, ++s2 == ORTH_SIZE ? s2 = 0 : 0){
			is_composite[m/WHEEL_SIZE] |= lut_masks[m%WHEEL_SIZE];
		}
	}
	free(is_composite);
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

