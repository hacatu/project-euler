#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX 120000

void radSieve(size_t max, uint64_t *rad, size_t *rfc, uint64_t *rfs, uint64_t *rfd){
	rad[0] = 0;
	*rfc = 0;
	for(uint64_t i = 1; i < max; ++i){
		rad[i] = 2 - i%2;//set rad to 2 for even numbers and 1 for odd numbers
	}
	for(uint64_t n = 3; n < max; ++n){
		if(rad[n] != 1){//n is not prime
			if(n > rad[n]){
				rfs[*rfc] = n;
				rfd[*rfc] = n/rad[n];
				++*rfc;
			}
			continue;
		}
		for(uint64_t m = n; m < max; m += n){
			rad[m] *= n;
		}
	}
}

uint64_t gcd(uint64_t a, uint64_t b){
	if(a < b){
		b %= a;
		if(!b){
			return a;
		}
	}
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

int main(void){
	uint64_t *rad = malloc(MAX*sizeof(uint64_t));//rad of all n
	uint64_t *rfs = malloc(MAX*sizeof(uint64_t));//array of n where rad[n] < n
	uint64_t *rfd = malloc(MAX*sizeof(uint64_t));//array of n/rad[n] for all n where rad[n] < n
	if(!(rad && rfs && rfd)){
		puts("Could not allocate memory!");
		return 1;
	}
	size_t rfc;
	radSieve(MAX, rad, &rfc, rfs, rfd);
	uint64_t n = 0, C = 0;
	for(uint64_t i = 0, c; i < rfc; ++i){
		c = rfs[i];
		for(uint64_t a = 1, b; 2*a < c; ++a){
			b = c - a;
			if(rad[a]*rad[b] >= rfd[i]){
				continue;
			}
			if(gcd(rad[b], rad[c]) != 1 || gcd(rad[c], rad[a]) != 1 || gcd(rad[a], rad[b]) != 1){
				continue;
			}
			++n;
			C += c;
		}
	}
	printf("%"PRIu64", %"PRIu64"\n", n, C);
	free(rad);
	free(rfs);
	free(rfd);
}

