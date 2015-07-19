#include <stdio.h>
#include <inttypes.h>

uint64_t digitSquares[10] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};
//the sum of the squares in 9999999 is 567
int ends[568] = {[1] = 1, [4] = 4};

uint64_t sumDigitSquares(uint64_t n){
	uint64_t s = 0;
	while(n){
		s += digitSquares[n%10];
		n /= 10;
	}
	return s;
}

int end(uint64_t n){
	n = sumDigitSquares(n);
	if(ends[n]){
		return ends[n];
	}
	return ends[n] = end(n);
}

int main(){
	uint64_t c = 0;
	for(uint64_t i = 2; i < 10000000; ++i){
		if(end(i) == 4){
			++c;
		}
	}
	printf("%"PRIu64"\n", c);
}

