#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

uint64_t *genPhi(uint64_t max){//phi(n) = n*prod(1 - 1/p, p divides n)
	uint64_t *phi = malloc(max*sizeof(uint64_t));
	if(!phi){
		return NULL;
	}
	for(uint64_t i = 1; i < max; ++i){//phi(n) = n*prod(1 - 1/p, p divides n)
		phi[i] = i;
	}
	for(uint64_t p = 2; p < max; ++p){//phi(n) = n*prod((p - 1)/p, p divides n)
		if(phi[p] == p){
			for(uint32_t n = p; n < max; n += p){
				phi[n] /= p;
				phi[n] *= p - 1;
			}
		}
	}
	return phi;
}

uint64_t calcHidden(uint64_t n){
	uint64_t *phi = genPhi(n + 1);
	if(!phi){
		return UINT64_MAX;
	}
	uint64_t ret = 0;
	for(uint64_t i = 1; i <= n; ++i){
		ret += phi[i];
	}
	free(phi);
	ret >>= 1;
	return 3*n*(n + 1) - 12*ret;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Invalid invocation, please use like:\n"
			"%s <hexagon size>\n", argv[0]);
		exit(1);
	}
	uint64_t n;
	sscanf(argv[1], "%"SCNu64, &n);
	printf("%"PRIu64": %"PRIu64"\n", n, calcHidden(n));
}

