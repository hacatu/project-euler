#include <stdio.h>
#include <stdlib.h>

#define MAX 10000000

int *fCountSieve(int max){
	int *ret = malloc((max + 1)*sizeof(int));
	if(!ret){
		return NULL;
	}
	for(int i = 1; i <= max; ++i){
		ret[i] = 1;
	}
	for(int p = 2; p <= max; ++p){
		if(ret[p] != 1){
			continue;
		}
		for(int m = p, e; m <= max; m += p){
			e = 1;
			for(int r = m/p; r%p == 0; r /= p, ++e);
			ret[m] *= e + 1;
		}
	}
	return ret;
}

int main(void){
	int *fCount = fCountSieve(MAX);
	int c = 0;
	for(int n = 2; n < MAX; ++n){
		if(fCount[n] == fCount[n + 1]){
			++c;
		}
	}
	printf("%i\n", c);
	free(fCount);
}

