#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define MAX_PERIM 10000

inline static int is_square(uint64_t n){
	uint64_t r = sqrtl(n);
	return n == r*r;
}

int main(void){
	uint64_t s = 0;
	for(uint64_t a = 3; a <= (MAX_PERIM >> 1) - 4; ++a){
		for(uint64_t b = 3; b <= (MAX_PERIM >> 1) - 1 - a; ++b){
			if(!is_square(a*a + b*b)){
				continue;
			}
			for(uint64_t c = (b >> 1) + 1; c <= ((MAX_PERIM - b) >> 1) - a && c < sqrtl(a*a + b*b*.25L); ++c){
				uint64_t k = (c*c << 2) - b*b;
				if(!is_square(k)){
					continue;
				}
				uint64_t d = a*a + c*c + a*sqrtl(k);
				if(!is_square(d)){
					continue;
				}
				//printf("%"PRIu64",%"PRIu64",%"PRIu64"\n", a, b, c);
				s += ((a + c) << 1) + b;
			}
		}
	}
	printf("%"PRIu64"\n", s);
}

