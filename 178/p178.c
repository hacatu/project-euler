/*
 * Pandigital step numbers - base 3 example
 * 0         c   d
 *          / \ /
 * 1   *   *   *
 *    / \ /
 * 2 a   b
 * B is the base (3 in the above example, 10 in
 * the problem).  b and c are positions in the number
 * a and d are the start and end digit.
 * [0, b] -> [0, B - 2]; [b, c - 1] -> [0, B - 2];
 * [c, L] -> [0, B - 1]
 * Then multiply by 2.
 * Memoize ways to complete
 * [0] -> [a]; [1, b - 1] -> [0, B - 2]; [b] - > [0]
 * and
 * [c] -> [B - 1]; [c + 1, L - 2] -> [0, B - 1]; [L - 1] -> [d]
 * That is, the number of ways to create a block of height B - 1
 * for all lengths and starts given it ends at 0.  These can be
 * used for the first two sections.  Also, the number of ways to
 * create a block of height B for all lengths and starts given it
 * starts at B - 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define B 10
#define L 10

int64_t ways_B2[L - B + 2][B - 2] = {{1}};
int64_t ways_B1[L - B + 1][B - 1] = {{1}};
int64_t ways_B[L - B + 1][B] = {{[B - 2]=1}};

void __attribute__((__constructor__)) _init_ways_B1(void){
	for(int64_t l = 1; l < L - B + 1; ++l){
		memcpy(ways_B1[l], ways_B1[l - 1] + 1, (B - 2)*sizeof(int64_t));
		for(int64_t a = 0; a < B - 1; ++a){
			ways_B1[l][a + 1] += ways_B1[l - 1][a];
		}
	}
}

void __attribute__((__constructor__)) _init_ways_B(void){
	for(int64_t l = 1; l < L - B; ++l){
		memcpy(ways_B[l], ways_B[l - 1] + 1, (B - 1)*sizeof(int64_t));
		for(int64_t a = 0; a < B; ++a){
			ways_B[l][a + 1] += ways_B[l - 1][a];
		}
	}
}

int64_t ways(int64_t a, int64_t b, int64_t c, int64_t d, int64_t l){
	return ways_B1[b][a]*ways_B1[c - b - 1][B - 2]*ways_B[l - c - 1][d];
}

int main(void){
	int64_t s = 0, s2 = 0;
	for(int64_t l = B; l <= L; ++l){
		printf("l: %"PRId64"\n", l);
		for(int64_t a = 1; a < B - 1; ++a){
			for(int64_t d = 0; d < B; ++d){
				for(int64_t b = a; b < l - 2*B + 2 + d; ++b){
					for(int64_t c = b + B - 1; c < l - B + 1 + d; ++c){
						s2 += ways(a, b, c, d, l);
					}
				}
			}
		}
		for(int64_t d = 0; d < B; ++d){
			printf(" d: %"PRId64"\n", d);
			for(int64_t b = 0; b < l - 2*B + 2 + d; ++b){
				printf("  b: %"PRId64"\n", b);
				for(int64_t c = b + B - 1; c < l - B + 1 + d; ++c){
					printf("   c: %"PRId64"\n", c);
					printf("   %"PRId64"*%"PRId64"*%"PRId64"\n", ways_B1[b][0], ways_B1[c - b - 1][B - 2], ways_B[l - c - 1][d]);
					s += ways(0, b, c, d, l);
				}
			}
		}
	}
	printf("%"PRId64"\n", 2*s2 + s);
}

