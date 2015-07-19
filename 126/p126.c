/* Project Euler 126
 * In each layer of the cuboid, the number of cubes required to cover it is the
 * faces plus the edges plus the corners.  For layer n, the area of the faces is
 * always 2*(a*b + b*c + c*a).  The area of the edges is 4*(a + b + c)*(n - 1).
 * The area of the corners is 4*(n - 2)*(n - 1), or 8* the n - 2 th triangular
 * number.  This give the overall formula
 * 2*(a*b + b*c + c*a + 2*(n - 1)*(a + b + c + n - 2)).
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX 20000

static inline uint64_t faceCubes(uint64_t a, uint64_t b, uint64_t c){
	return 2*(a*b + b*c + c*a);
}

static inline uint64_t edgeCubes(uint64_t a, uint64_t b, uint64_t c, uint64_t n){
	return 4*(a + b + c)*(n - 1);
}

static inline uint64_t cornerCubes(uint64_t n){
	return 4*(n - 2)*(n - 1);
}

int main(void){
	uint64_t *ways = calloc(MAX, sizeof(uint64_t));
	if(!ways){
		puts("failed to allocate memory!");
		return 1;
	}
	for(uint64_t a = 1; 2*(2*a + 1) < MAX; ++a){
		for(uint64_t b = 1; b <= a && 2*(a*b + a + b) < MAX; ++b){
			for(uint64_t c = 1; c <= b && 2*(a*b + b*c + c*a) < MAX; ++c){
				for(uint64_t n = 1, N; ; ++n){
					N = faceCubes(a, b, c) + edgeCubes(a, b, c, n) + cornerCubes(n);
					if(N >= MAX){
						break;
					}
					++ways[N];
				}
			}
		}
	}
	for(uint64_t n = 120; n < MAX; ++n){
		if(ways[n] == 1000){
			printf("%"PRIu64"\n", n);
			break;
		}
	}
	free(ways);
}

