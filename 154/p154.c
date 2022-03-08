#include <stdio.h>
#include <inttypes.h>

#define LAYER 200000
#define MOD 1000000000000

/* bicoeff1(a, b) = (a + b)!/a!b! = (a + b)*(a + b - 1)*...*(b + 1)/a!
 */
uint64_t bicoeff1(uint64_t a, uint64_t b){
	uint64_t ret = 1;
	for(uint64_t i = 1; i <= a; ++i){
		ret *= b + i;
		ret /= i;
	}
	return ret;
}

/* (n i,j,k) = (i i)(i + j j)(i + j + k k) = (i + j j)(i + j + k k)
 */
uint64_t tricoeff(uint64_t i, uint64_t j, uint64_t k){
	return bicoeff1(i, j)*bicoeff1(i + j, k);
}

//note: will not work if m == 1
uint64_t countLayer(uint64_t n, uint64_t m){
	uint64_t s = 0;
	#pragma omp parallel for reduction(+:s)
	for(uint64_t i = 1; i <= n/2; ++i){
		if(tricoeff(i, i, n - i - i)%m == 0){
			s += 3;
		}
	}
	if(n%3 == 0 && tricoeff(n/3, n/3, n/3)%m == 0){
		s -= 2;
	}
	#pragma omp parallel for reduction(+:s)
	for(uint64_t i = n/2 + 1; i < n; ++i){
		for(uint64_t j = 0, k = n - i; j < k; ++j, --k){
			if(tricoeff(i, j, k)%m == 0){
				s += 6;
			}
		}
	}
	return s;
}

int main(void){//note this loop will not work if n is a multiple of 3.
	printf("%"PRIu64"\n", countLayer(LAYER, MOD));
}

