#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

const uint32_t factorials[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

uint32_t digitFactorials[10000000];

int8_t chainDepths[10000000] = {[1] = 1,
								[2] = 1,
								[145] = 1,
								[169] = 3,
								[871] = 2,
								[872] = 2,
								[1454] = 3,
								[40585] = 1,
								[45361] = 2,
								[45362] = 2,
								[363601] = 3
							   };

uint32_t digitFactorial(uint32_t n){
	uint32_t s = 0;
	do{
		s += factorials[n%10];
		n /= 10;
	}while(n);
	return s;
}

uint8_t chainDepth(uint32_t n){
	if(chainDepths[n] == 0){//not yet computed
		return chainDepths[n] = 1 + chainDepth(digitFactorials[n]);
	}
	return chainDepths[n];
}

int main(int argc, char **argv){
	uint32_t count = 0;
	for(uint32_t i = 1; i < 10000000; ++i){
		digitFactorials[i] = digitFactorial(i);
	}
	for(uint32_t i = 1; i < 1000000; ++i){
		if(chainDepth(i) == 60){
			++count;
		}
	}
	printf("c :%"PRIu8"\n", count);
}

