#include <stdio.h>
#include <complex.h>
#include <math.h>

double inctr_isopp(double a, double b, double c){
	double x = (a*(a + b + c) - b*c)/(a + b);
	double y = sqrt((a + c)*(a + c) - x*x);
	double k4 = (1./a + 1./b + 1./c + 2.*sqrt(1./(a*b) + 1./(b*c) + 1./(a*c)));
	double complex z1 = 0., z2 = a + b, z3 = x + y*I;
	double complex z4 = ((a + b)/b + z3/c + 2.*csqrt((a + b)*z3/(b*c)))/k4;
	double complex z5 = (a + b)*(a + c + x)/(2.*(a + b + c)) + (a + b)*y/(2.*(a + b + c))*I;
	return cabs(z4 - z5);
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
	double s = 0;
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
	printf("%.15f\n", s/n);
}

