#include <stdio.h>
#include <stdint.h>

uint16_t collatzs[1156908673] = {[1] = 1};
uint64_t collatz(uint64_t n){
	if(collatzs[n]){
		return collatzs[n];
	}
	return collatzs[n] = 1 + collatz(n&1 ? 3*n + 1 : n>>1);
}

int main(){
	uint64_t m = 0, n;
	uint16_t l;
	for(uint64_t i = 500001; i < 1000000; i += 2){
		if((l = collatz(i)) > m){
			m = l;
			n = i;
		}
	}
	printf("%uli\n", n);
}
