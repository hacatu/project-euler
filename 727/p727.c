#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

long double inctr_isopp(long double a, long double b, long double c){
	long double x = (a*(a + b + c) - b*c)/(a + b);
	long double y = sqrtl((a + c)*(a + c) - x*x);
	//printf("z3:(%Lf,%Lf)\n", x, y);
	long double k4 = (1.L/a + 1.L/b + 1.L/c + 2.L*sqrtl(1.L/(a*b) + 1.L/(b*c) + 1.L/(a*c)));
	long double complex z1 = 0.L, z2 = a + b, z3 = x + y*I;
	long double complex z4 = ((a + b)/b + z3/c + 2.L*csqrtl((a + b)*z3/(b*c)))/k4;
	long double complex z5 = (a + b)*(a + c + x)/(2.L*(a + b + c)) + (a + b)*y/(2.L*(a + b + c))*I;
	//printf("r4=%Lf\nr5=%Lf\n", 1.L/k4, (a + b)*y/(2.L*(a + b + c)));
	//printf("z4:(%Lf,%Lf)\nz5:(%Lf,%Lf)\n", creall(z4), cimagl(z4), creall(z5), cimagl(z5));
	return cabsl(z4 - z5);
}

int gcd(int a, int b){
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

int main(){
	long double s = 0;
	int n = 0;
	#pragma omp parallel for reduction(+:s,n)
	for(int a = 1; a < 99; ++a){
		for(int b = a + 1; b < 100; ++b){
			int d = gcd(a, b);
			for(int c = b + 1; c < 101; ++c){
				if(gcd(c, d) != 1){
					continue;
				}
				++n;
				s += inctr_isopp(a, b, c);
			}
		}
	}
	printf("%.18Lf\n", s/n);
}

//3.640391406429889522
//3.64039141

