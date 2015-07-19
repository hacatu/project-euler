#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000000
#define WHEEL_SIZE 210
#define ORTH_SIZE 48
#define CHARS_PER_WHEEL 6

#define OVERESTIMATE_PRIME_COUNT(n) ((size_t)(1.25506*n/log(n)))//this constant is described at oeis 209883

HAC_VEC_T_NEW(uint64_t);

const uint8_t offsets[ORTH_SIZE] = {10,2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,};
const uint8_t lut_offsets[WHEEL_SIZE] = {[1]=0,[11]=0,[13]=0,[17]=0,[19]=0,[23]=0,[29]=0,[31]=0,[37]=1,[41]=1,[43]=1,[47]=1,[53]=1,[59]=1,[61]=1,[67]=1,[71]=2,[73]=2,[79]=2,[83]=2,[89]=2,[97]=2,[101]=2,[103]=2,[107]=3,[109]=3,[113]=3,[121]=3,[127]=3,[131]=3,[137]=3,[139]=3,[143]=4,[149]=4,[151]=4,[157]=4,[163]=4,[167]=4,[169]=4,[173]=4,[179]=5,[181]=5,[187]=5,[191]=5,[193]=5,[197]=5,[199]=5,[209]=5,};
const uint8_t lut_masks[WHEEL_SIZE] = {[1]=0x1,[11]=0x2,[13]=0x4,[17]=0x8,[19]=0x10,[23]=0x20,[29]=0x40,[31]=0x80,[37]=0x1,[41]=0x2,[43]=0x4,[47]=0x8,[53]=0x10,[59]=0x20,[61]=0x40,[67]=0x80,[71]=0x1,[73]=0x2,[79]=0x4,[83]=0x8,[89]=0x10,[97]=0x20,[101]=0x40,[103]=0x80,[107]=0x1,[109]=0x2,[113]=0x4,[121]=0x8,[127]=0x10,[131]=0x20,[137]=0x40,[139]=0x80,[143]=0x1,[149]=0x2,[151]=0x4,[157]=0x8,[163]=0x10,[167]=0x20,[169]=0x40,[173]=0x80,[179]=0x1,[181]=0x2,[187]=0x4,[191]=0x8,[193]=0x10,[197]=0x20,[199]=0x40,[209]=0x80,};

int main(void){
	clock_t start = clock();
	char *is_composite = calloc(MAX/WHEEL_SIZE*ORTH_SIZE/8, sizeof(char));
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, OVERESTIMATE_PRIME_COUNT(MAX));
	HAC_VEC_PUSHR(uint64_t, &primes, 2);
	HAC_VEC_PUSHR(uint64_t, &primes, 3);
	HAC_VEC_PUSHR(uint64_t, &primes, 5);
	for(uint64_t n = 7, s1 = 1; n < MAX; n += offsets[s1], ++s1 == ORTH_SIZE ? s1 = 0 : 0){
		if(is_composite[n/WHEEL_SIZE*CHARS_PER_WHEEL + lut_offsets[n%WHEEL_SIZE]]&lut_masks[n%WHEEL_SIZE]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		for(uint64_t m = n*n, s2 = s1; m < MAX; m += offsets[s2]*n, ++s2 == ORTH_SIZE ? s2 = 0 : 0){
			is_composite[m/WHEEL_SIZE*CHARS_PER_WHEEL + lut_offsets[n%WHEEL_SIZE]] |= lut_masks[m%WHEEL_SIZE];
		}
	}
	free(is_composite);
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

