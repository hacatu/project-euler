#include <stdio.h>
#include <string.h>
#include <inttypes.h>

char digits[10];
 
char isUsed(int n){
	return (n < 1 || n > 9 || digits[n]);
}

#define LOOP_DIGITS(x, y, z) for(int y = 1, x; y < k - z; ++y)
 
#define TRY_DIGIT(x, y, z)                                             \
x = k - y - z;                                                         \
if(isUsed(x)){                                                         \
	digits[y] = 0;                                                     \
	continue;                                                          \
}                                                                      \
digits[x] = 1;

#define TRY_DIGITS(x, y, z)                                            \
if(isUsed(y)){                                                         \
	continue;                                                          \
}                                                                      \
digits[y] = 1;                                                         \
TRY_DIGIT(x, y, z)


#define CLEAR_DIGITS(m, n)                                             \
digits[m] = 0;                                                         \
digits[n] = 0;

#define PRINT_TRIPLE(x, y, z) printf("%i%i%i", x, y, z);

#define PRINT_RING(f, b, g, c, h, d, i, e, j, a)                       \
    if(f == min){                                                      \
        PRINT_TRIPLE(f, b, c)                                          \
		PRINT_TRIPLE(g, c, d)                                          \
		PRINT_TRIPLE(h, d, e)                                          \
		PRINT_TRIPLE(i, e, a)                                          \
		PRINT_TRIPLE(j, a, b)                                          \
    }                                                                  \

#define PRINT_SOLUTION()                                               \
{                                                                      \
	int min = g < f ? g : f;                                           \
	if(h < min){                                                       \
		min = h;                                                       \
	}                                                                  \
	if(i < min){                                                       \
		min = i;                                                       \
	}                                                                  \
	PRINT_RING(f, b, g, c, h, d, i, e, 10, a)                          \
	PRINT_RING(g, c, h, d, i, e, 10, a, f, b)                          \
	PRINT_RING(h, d, i, e, 10, a, f, b, g, c)                          \
	PRINT_RING(i, e, 10, a, f, b, g, c, h, d)                          \
	puts("");                                                          \
}                                                                      \

/* 10
 *    \
 *      a     f
 *    /   \  /
 *  e       b
 * / \     /
 *i   d --c -- g
 *     \
 *      h
 * i = k - a - e
 * h = k - d - e
 * g = k - c - d
 * f = k - b - c
 * b = k - 10 - a
 * variables: k, a, c, d, e
 * k: 13 , 14 , 15     , 16     , 17         , 18
 * a: 1 2, 1 3, 1 2 3 4, 1 2 4 5, 1 2 3 4 5 6, 1 2 3 5 6 7
 * b: 2 1, 3 1, 4 3 2 1, 5 4 2 1, 6 5 4 3 2 1, 7 6 5 3 2 1
 */

int main(){
	for(int k = 13, i; k <= 18; ++k){
		printf("k: %i\n", k);
		memset(digits, 0, 10*sizeof(char));
		LOOP_DIGITS(b, a, 10){
			digits[a] = 1;
			TRY_DIGIT(b, a, 10) //set b to k - a - 10 and clear a and continue if it fails
			LOOP_DIGITS(f, c, b){
				TRY_DIGITS(f, c, b)
				LOOP_DIGITS(g, d, c){
					TRY_DIGITS(g, d, c)
					LOOP_DIGITS(h, e, d){
						TRY_DIGITS(h, e, d)
						i = k - a - e;
						if(isUsed(i)){
							CLEAR_DIGITS(h, e)
							continue;
						}
						PRINT_SOLUTION()
						CLEAR_DIGITS(h, e)
					}
					CLEAR_DIGITS(g, d)
				}
				CLEAR_DIGITS(f, c)
			}
			CLEAR_DIGITS(b, a)
		}
	}
}

