#include <stdio.h>
#include <hac_vec.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#define MAX 1000000

HAC_VEC_T_NEW(uint64_t);

int main(void){
	clock_t start = clock();
	HAC_VEC_T(uint64_t) primes = HAC_VEC_NEW(uint64_t, MAX/2);
	for(uint64_t n = 3; n < MAX; n += 2){
		for(uint64_t i = 0, p = primes.a[i]; p < sqrt(n) && i < primes.n; p = primes.a[++i]){
			if(!(n%primes.a[i])){
				goto NOT_PRIME;
			}
		}
		HAC_VEC_PUSHR(uint64_t, &primes, n);
		NOT_PRIME:;
	}
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

