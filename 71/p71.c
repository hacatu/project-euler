#include <stdio.h>
#include <inttypes.h>

int main(){
	uint64_t targetN = 3, targetD = 7, maxN = 2, maxD = 5;
	for(uint64_t n, d = 9; d <= 1000000; ++d){
		if(d % 7 == 0){
			continue;
		}
		n = targetN*d/targetD;
		if(n*maxD > maxN*d){
			maxN = n;
			maxD = d;
		}
	}
	printf("%" PRIu64 "\n", maxN);
}

