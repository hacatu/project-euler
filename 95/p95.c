#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../more prime sieves/sigmaSieve.h"

int *sigma;

int cyclens[1000001] = {[1] = -1};//1 = perfect #, 2 = amicable pair, >2 = cycle, 0 = unfilled, -max = exceeds 1m, -n = working, -1 = aspiring

int cyclen(int n){//!6!6
	//printf("cyclen(%i)\n", n);
	static int depth;
	if(n == 7524){
		printf("depth: %i\n", depth);
		//printf("segfault %i\n", sigma[-1]);
	}
	int len;
	if(n > 1000000){
		//printf("cyclen: n too high\n");
		depth = -1;
		return INT_MIN;//-max
	}
	//printf("cyclens[n]: %i\n", cyclens[n]);
	switch(cyclens[n]){
		case INT_MIN://-max, exceeds
			//printf("unbounded\n");
			depth = -1;
			return INT_MIN;//-max
		case 0://0, unfilled
			cyclens[n] = --depth;//!cyclens[6] = -2
			//printf("unfilled, depth: %i\n", depth);
			//printf("recursively call cyclen...\n");
			if(sigma[n] == 7524){
				printf("n: %i\n", n);
				//printf("segfault %i\n", sigma[-1]);
			}
			len = cyclen(sigma[n]);//!recursion!returns 1 with depth set to -2
			//printf("recursive call complete, len: %i\n", len);
			if(len == -1 || len == INT_MIN){
				//printf("number is not part of a cycle under 1000000\n");
				depth = -1;
				return cyclens[n] = len;
			}
			//printf("has chain ended? depth: %i cyclens[n]: %i\n", depth, cyclens[n]);
			if(depth >= cyclens[n]){
				//printf("the chain has not ended, returning len: %i\n", len);
				if(cyclens[n] == -2){//should this be cyclens[n]?
					//printf("the chain was entered here, resetting depth to -1\n");
					depth = -1;
				}
				return cyclens[n] = len;
			}
			//printf("the chain has ended.\n");
			depth = -1;
			return cyclens[n] = -1;
			
		default:
			if(cyclens[n] < -1){//working//!!-2
				//printf("found tail\n");
				len = cyclens[n] -depth + 1;//-2 --2 +1 = 1
				depth = cyclens[n];// -2
				return len;//1
			}
			depth = -1;
			return cyclens[n];
	}
}

int main(){
	printf("INT_MIN == %i\n", INT_MIN);
	//puts("clearing cyclen");
	printf("cyclen(1000001): %i\n", cyclen(1000001));
	//puts("cyclen cleared, depth should be -1");
	sigma = sigmaSieve(1000000);
	if(!sigma){
		return 1;
	}
	puts("Finished sigmaSieve");
	
	int maxlen = 0, maxi;
	for(int i = 1; i < 1000000; ++i){
		if(cyclen(i) > maxlen){
			maxlen = cyclen(i);
			maxi = i;
			printf("new maxlen = %i, maxi = %i\n", maxlen, maxi);
		}
	}
	puts("Finished cyclen");
	
	printf("%i\n", maxi);
	
	printf("%i\n", cyclen(7524));
	/*
	printf("%i\n", sigma[7524]);
	printf("%i\n", sigma[14316]);
	printf("%i\n", sigma[19116]);
	printf("%i\n", sigma[31704]);
	*/
	free(sigma);
}

