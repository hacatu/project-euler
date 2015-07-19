#include <stdio.h>
#include <inttypes.h>

int main(void){
	uint64_t s = 0;
	for(uint64_t a = 3, m, r; a <= 1000; ++a){
		m = 2;
		for(uint64_t n = 1; n <= 2*a; n +=2){
			r = 2*n*a%(a*a);
			if(r > m){
				m = r;
			}
		}
		s += m;
	}
	printf("%"PRIu64"\n", s);
}

