#include <stdio.h>
#include <inttypes.h>

#define MAX 60000

int64_t partitions[MAX + 1] = {[0] = 1, [1 ... MAX] = -1};

uint64_t partition(int n){
	if(n < 0){
		return 0;
	}
	if(partitions[n] == -1){
		partitions[n] = 0;
		for(int k = 1, a;; ++k){
			if(n < (k*(3*k - 1)>>1)){
				break;
			}
			a = partition(n - ((k*(3*k - 1))>>1));
			if(k&1){
				partitions[n] += a;
			}else{
				partitions[n] -= a;
			}
			if(n < (k*(3*k + 1)>>1)){
				break;
			}
			a = partition(n - ((k*(3*k + 1))>>1));
			if(k&1){
				partitions[n] += a;
			}else{
				partitions[n] -= a;
			}
		}
		partitions[n] %= 1000000;
	}
	return partitions[n];
}

int main(){
	int n = 1;
	for(; partition(n); ++n);
	printf("%i\n", n);
}

