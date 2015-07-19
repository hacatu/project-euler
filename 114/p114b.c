#include <stdio.h>
#include <inttypes.h>

/* If you use recursion, all of your problems will go away.
 * red-ended blocks: reb(1) = reb(2) = 0; reb(3) = 1
 * reb(n) = E(reb(i)*reb(j);i,j;i+j<n)+1
 * 
 * blocks: b(n) = E(reb(n-i-j);i+j<n)+1
 */

#define M 3
#define MAX 50

uint64_t _reb[MAX + 1] = {[M ... 2*M]=1};

uint64_t reb(uint64_t n){
	if(_reb[n]){
		return _reb[n];
	}
	uint64_t c = 1;
	for(uint64_t i = M; i < n - M; ++i){
		c += reb(i)*(n - M - i);
	}
	return _reb[n] = c;
}

uint64_t b(uint64_t n){
	uint64_t c = 1;
	for(uint64_t i = 0; i <= n - M; ++i){
		c += reb(n - i)*(i + 1);
	}
	return c;
}

int main(void){
	printf("%"PRIu64"\n", b(MAX));
}

