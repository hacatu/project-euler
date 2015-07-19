#include <stdio.h>
#include <inttypes.h>
#include <math.h>

char isSquare(uint64_t n){
	uint64_t r = sqrt(n);
	return n == r*r ? 1 : 0;
}

int main(){
	uint64_t maxMinX = 0, maxMinD = 0;
	for(uint64_t d = 61, minX, y2; d <= 61; ++d){
		if(isSquare(d)){
			continue;
		}
		printf("d: %" PRIu64 "\n", d);
		for(minX = sqrt(d); minX; ++minX){//checks for overflow
			printf("x: %" PRIu64 "\n", minX);
			if((minX*minX - 1)%d == 0){
				y2 = (minX*minX - 1)/d;
				if(y2 && isSquare(y2)){
					break;
				}
			}
		}
		//printf("%" PRIu64 "^2 - %" PRIu64 "*%" PRIu64 "^2 = 1\n", minX, d, (uint64_t)sqrt(y2));
		if(minX > maxMinX){
			maxMinX = minX;
			maxMinD = d;
			printf("New maxMinD: %" PRIu64 "\n", maxMinD);
		}
	}
	printf("%" PRIu64 "\n", maxMinD);
}

