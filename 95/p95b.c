#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../more prime sieves/sigmaSieve.h"

int *sigma;

int cyclens[1000001] = {[1] = -1};//1 = perfect #, 2 = amicable pair, >2 = cycle, 0 = unfilled, -max = exceeds 1m, -n = working, -1 = aspiring

int cyclen(int n){
	static int depth = -1;
	int len;
	if(n > 1000000){
		return INT_MIN;
	}
	switch(cyclens[n]){
		case INT_MIN:
			return INT_MIN;
		case 0:
			cyclens[n] = --depth;
			len = cyclen(sigma[n]);
			if(len == -1 || len == INT_MIN){
				depth = -1;
				return cyclens[n] = len;
			}
			if(depth == -1){
				return cyclens[n] == -1;
			}
			if(depth >= cyclens[n]){
				return cyclens[n] = len;
			}
			depth = -1;
			return cyclens[n] = -1;
		default:
			if(cyclens[n] < -1){
				len = cyclens[n] -depth + 1;
				depth = cyclens[n];
				return len;
			}
			depth = -1;
			return cyclens[n];
	}
}

int main(){
	sigma = sigmaSieve(1000000);
	if(!sigma){
		return 1;
	}
	puts("Finished sigmaSieve");
	int printed = 0;
	int maxlen = 0, maxi;
	for(int i = 1; i < 1000000; ++i){
		if(cyclen(i) > maxlen){
			maxlen = cyclen(i);
			maxi = i;
			printf("new maxlen = %i, maxi = %i\n", maxlen, maxi);
		}
		if((sigma[i] == 284) && (printed < 2)){
			printed++;
			printf("%i\n", i);
		}
	}
	puts("Finished cyclen");
	
	printf("%i\n", maxi);
	free(sigma);
}

