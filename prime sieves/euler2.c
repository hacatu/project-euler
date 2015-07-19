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
	for(uint64_t a = 3; a < MAX; a += 2){
		if(isComposite[a]){
			continue;
		}
		HAC_VEC_PUSHR(uint64_t, &primes, a);
		for(uint64_t b = a, m; a*b < MAX; b += 2){
			if(isComposite[b]){
				continue;
			}
			m = a*b;
			do{
				isComposite[m] = 1;
				m *= a;
			}while(m < MAX);
		}
	}
	HAC_VEC_DELETE(uint64_t, &primes);
	printf("%f\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

