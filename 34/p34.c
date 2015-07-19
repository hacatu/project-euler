#include <stdio.h>
#include <inttypes.h>

const uint64_t factorial[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

uint64_t factorialDigitSum(uint64_t n){
	uint64_t s = 0;
	while(n){
		s += factorial[n%10];
		n /= 10;
	}
	return s;
}

int main(){
	uint64_t sum = 0;
	for(uint64_t n = 145; n < 10000000; ++n){
		if(n == factorialDigitSum(n)){
			printf("%" PRIu64 "\n", n);
			sum += n;
		}
	}
	printf("%" PRIu64 "\n", sum);
}

