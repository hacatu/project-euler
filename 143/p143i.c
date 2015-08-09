#include <stdio.h>
#include <math.h>

#define MIN(a, b) ({__auto_type _a = a; __auto_type _b = b; _a > _b ? _b : _a;})

int isSquare(int n){
	int r = sqrt(n);
	return n == r*r;
}

int main(void){
	int s = 0;
	int muls[120001] = {0};
	#pragma omp parallel for ordered reduction(+:s)
	for(int l = 3; l <= 120000; ++l){
		if(muls[l]){
			continue;
		}
		for(int p = 1; p < l - 1; ++p){
			for(int q = (l - p + 1)/2; q < MIN(p + 1, l - p); ++q){
				int r = l - p - q;
				/* a^2 = q^2 + r^2 - 2*q*r*cos(CTB)
				 * a^2 = q^2 + r^2 - 2*q*r*cos(2*pi/3)
				 * a^2 = q^2 + r^2 - 2*q*r*(-1/2)
				 * a^2 = q^2 + q*r + r^2
				 */
				if(isSquare(p*p + p*q + q*q) && isSquare(r*r + r*p + p*p) && isSquare(q*q + q*r + r*r)){
					printf("%i, %i, %i, %i\n", p, q, r, l);
					int m = l;
					do{
						s += m;
						muls[m] = 1;
						m += l;
					}while(m <= 120000);
					goto CONTINUE_L;
				}
			}
		}
		CONTINUE_L:;
	}
	printf("%i\n", s);
}

