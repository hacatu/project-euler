#include <stdio.h>
#include <inttypes.h>

#define MAX 100000000

/* Pythagorean triples (a, b, c) | (b - a)|c | a + b + c < 100000000
 * Generating pythagorean triples:
 * a = k*(m^2 - n^2), b = k*(2*m*n), c = k*(m^2 + n^2), perim = 2*k*(m^2 + m*n)
 * m > n.  m and n are coprime.  Start with m - n is odd.
 */

uint64_t gcd(uint64_t a, uint64_t b){
	while(1){
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
		a %= b;
	}
}

int main(void){
	uint64_t c = 0;
	for(uint64_t m = 1; 2*(m*m + m) < MAX; ++m){
		for(uint64_t n = m&1 ? 2 : 1, a0, b0, c0; n < m; n += 2){
			if(2*(m*m + m*n) >= MAX){
				break;
			}
			if(gcd(m, n) != 1){
				continue;
			}
			a0 = m*m - n*n;
			b0 = 2*m*n;
			c0 = m*m + n*n;
			if(c0%(b0 - a0)){
				continue;
			}
			c += MAX/(a0 + b0 + c0);
		}
	}
	printf("%"PRIu64"\n", c);
}

