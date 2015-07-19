#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MIN 1000000
#define MAX 100

void addR(const uint64_t *r1, uint64_t *r2, int r, int *c){
	r2[0] = r;
	if(!(r&1)){//if r is even, there is a middle element.
		//Set it now because only half of the array is kept
		r2[r/2 - 1] = r1[r/2 - 2] << 1;
	}
	for(int i = 1; i < (r - 1)/2; ++i){
		r2[i] = r1[i - 1] + r1[i];
		if(r2[i] > MIN){
			r2[i] = MIN;
			*c += r - 1 - 2*i;
			return;
		}
	}
}

int main(){
	//for PE's numbers, these arrays could be 11 long, but they are MAX = 100 for generality.
	uint64_t ro[MAX] = {3};//row 3
	uint64_t re[MAX] = {4, 6};//row 4
	int c = 0;
	for(int r = 5; r <= MAX; ++r){
		addR(re, ro, r, &c);
		++r;
		if(r > MAX){
			break;
		}
		addR(ro, re, r, &c);
	}
	printf("found answer: %i\n", c);
}

