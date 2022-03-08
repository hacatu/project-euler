#include <stdio.h>
#include <stdlib.h>
#include <primesieve.h>

#define MAX 100000000

int main(void){
	size_t size;
	int *primes = primesieve_generate_primes(1, MAX/2, &size, INT_PRIMES);
	int c = 0;
	for(int i = 0, t = size - 1; i < size; ++i){
		while(primes[i]*primes[t] > MAX)--t;
		if(t < i)break;
		c += t - i + 1;
	}
	printf("%i\n", c);
	primesieve_free(primes);
}

