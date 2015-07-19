#include <stdio.h>
#include <inttypes.h>

#define MAX 416

uint64_t partitions[MAX + 1] = {[0] = 1};

uint64_t partition(int n){
	if(n < 0){
		return 0;
	}
	if(!partitions[n]){
		for(int k = 1;; ++k){
			int a = (k&1?1:-1)*(partition(n - ((k*(3*k - 1))>>1)));
			if(!a){
				break;
			}
			partitions[n] += a;
			a = (k&1?1:-1)*(partition(n - ((k*(3*k + 1))>>1)));
			if(!a){
				break;
			}
			partitions[n] += a;
		}
	}
	return partitions[n];
}

int main(){
	printf("%"PRIu64"\n", partition(MAX));
}

