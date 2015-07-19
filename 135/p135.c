#include <stdio.h>
#include <math.h>

/* x^2 - y^2 - z^2 = n
 * if x, y, and z form an arithmetic progression, then x is the largest and the order of y and z does not matter so let
 * x = a + d, y = a, z = a - d
 * (a + d)^2 - a^2 - (a - d)^2 = n
 * a^2 + 2*a*d + d^2 - a^2 - a^2 + 2*a*d - d^2 = n
 * 4*a*d - a^2 = n
 * I can loop over a and then have 
 * d = (n + a^2)/(4*a)
 * This implies a|n and 4|((n/a) + a).  Do a <= sqrt(n) and test a and n/a for all a|n.
 */

int solutions(int n){
	int s = 0, r = sqrt(n);
	for(int a = 1, t, d; a <= r; ++a){
		if(n%a){
			continue;
		}
		t = n/a;
		if(!((t + a)%4)){
			d = (t + a)/4;
			if(d < a){
				//printf("%i\n", a);
				++s;
			}
			if(d < t && t != a){
				//printf("%i\n", t);
				++s;
			}
		}
	}
	return s;
}

int main(void){
	int c = 0;
	for(int n = 1155; n < 1000000; ++n){
		if(solutions(n) == 10){
			++c;
		}
	}
	printf("%i\n", c);
}

