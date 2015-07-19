#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000

HAC_VEC_T_NEW(uint64_t);

int main(void){
	clock_t start = clock();
	char isComposite[MAX] = {0};
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, MAX/2);
	for(uint64_t n = 5, s1 = 2; n < MAX; n += s1, s1 = 6 - s1){
		if(isComposite[n]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		for(uint64_t m = n*n, s2 = s1; m < MAX; m += s2*n, s2 = 6 - s2){
			isComposite[m] = 1;
		}
	}
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

