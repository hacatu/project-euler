#include <stdio.h>
#include <stdlib.h>

/* x^2 - y^2 - z^2 = n
 * if x, y, and z form an arithmetic progression, then x is the largest and the order of y and z does not matter so let
 * x = a + d, y = a, z = a - d
 * (a + d)^2 - a^2 - (a - d)^2 = n
 * a^2 + 2*a*d + d^2 - a^2 - a^2 + 2*a*d - d^2 = n
 * 4*a*d - a^2 = n
 * d = (n + a^2)/(4*a)
 * This implies a|n and 4|((n/a) + a).
 */

int main(void){
	int *s = calloc(1000000, sizeof(int));
	if(!s){
		puts("Could not allocate memory!");
		return 1;
	}
	for(int a = 2; a < 1000000; ++a){
		for(int d = a/4, n; d < a; ++d){
			n = 4*a*d - a*a;
			if(n <= 0){
				continue;
			}
			if(n >= 1000000){
				break;
			}
			++s[n];
		}
	}
	int c = 0;
	for(int i = 1; i < 1000000; ++i){
		if(s[i] == 10){
			++c;
		}
	}
	free(s);
	printf("%i\n", c);
}

