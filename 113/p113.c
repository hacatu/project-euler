#include <stdio.h>
#include <inttypes.h>

uint64_t combinations(uint64_t n, uint64_t r){
	uint64_t c, d;
	if(2*r < n){
		r = n - r;
	}
	if(r > n){
		return 0;
	}
	for(c = 1, d = 1; n > r; --n, ++d){
		c = c*n/d;
	}
	return c;
}

uint64_t npart(uint64_t b, uint64_t p){
	return combinations(b + p - 1, b);
}

uint64_t inc(uint64_t b, uint64_t p){
	return npart(b, p + 1) - 1;
}

uint64_t dec(uint64_t b, uint64_t p){
	uint64_t c = 0;
	for(uint64_t l = p; l; --l){
		c += inc(b, l);
	}
	return c;
}

uint64_t nonBouncy(uint64_t b, uint64_t p){
	return inc(b, p) + dec(b, p) - b*p;
}

int main(void){
	printf("%"PRIu64"\n", nonBouncy(9, 100));
}

