#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

uint64_t area2_sleg2(uint64_t leg, uint64_t base){
	uint64_t p = 2*leg + base;
	return p/*(s - leg)*(s - leg)*/*(p - 2*base);//Hero's formula with the square part removed, * 4
}

int isSquare4(uint64_t n){
	if(n&0b11){
		return 0;
	}
	n /= 4;
	uint64_t r = sqrt(n);//Might not work for big n because doubles have 53 bits and uint64s have 64
	return r*r == n;
}

int main(){
	uint64_t c = 0;
	for(uint64_t leg = 2; leg < 333333334; ++leg){
		if(isSquare4(area2_sleg2(leg, leg - 1))){
			printf("%"PRIu64"\n", area2_sleg2(leg, leg - 1));
			c += 3*leg - 1;
		}
		if(isSquare4(area2_sleg2(leg, leg + 1))){
			printf("%"PRIu64"\n",area2_sleg2(leg, leg + 1));
			c += 3*leg + 1;
		}
	}
	printf("%"PRIu64"\n", c);
}

