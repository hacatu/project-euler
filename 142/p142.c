/* x + y + z
 * x > y > z
 * x + y, x - y, x + z, x - z, y + z, y - z are perfect squares.
 * a = x + y
 * b = x + z
 * c = y + z
 * x = (+ a + b - c)/2 = s - c
 * y = (+ a - b + c)/2 = s - b
 * z = (- a + b + c)/2 = s - a
 * s = x + y + z = (+ a + b + c)/2
 * c < b < a
 * note a, b, and c are square so actually loop over their square roots.
 * x - y = b - c
 * x - z = a - c
 * y - z = a - b
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>

int isSquare(uint64_t n){
	uint64_t r = sqrtl(n);
	return n == r*r;
}

uint64_t firstC(uint64_t a, uint64_t b){
	uint64_t c = sqrtl(a*a - b*b);
	return c + 2 - (a + b + c)%2;
}

int main(void){
	for(uint64_t a = 3;; ++a){
		for(uint64_t b = a/sqrtl(2); b < a; ++b){
			for(uint64_t c = firstC(a, b); c < b; c += 2){
				if(isSquare(b*b - c*c) && isSquare(a*a - c*c) && isSquare(a*a - b*b)){
					printf("%"PRIu64"\n", (a*a + b*b + c*c)/2);
					return 0;
				}
			}
		}
	}
}

