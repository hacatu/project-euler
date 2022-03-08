#define _GNU_SOURCE
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

double area(double n){
	double y = (n + 1. - sqrt(2.*n))/(n*n + 1);
	double x = n*y;
	double u = x - 1.;
	double a = x*y - 2*u + sqrt(1 - u*u)*u + asin(u);
	printf("area(%f): (%f, %f) -> %.10f\n", n, x, y, a);
	return a;
}

uint32_t find_n(double r){
	uint32_t b = ceil(1./((1. - M_PI_4)*r));
	double m = r*(2. - M_PI_2);
	printf("target: %.10f\n", m);
	uint32_t a = 1, n;
	while(b - a > 7){
		printf("searching [%"PRIu32", %"PRIu32")\n", a, b);
		uint32_t c = (a + b - 1) >> 1;
		if(area(c) < m){
			b = c + 1;
		}else{
			a = c + 1;
		}
	}
	for(n = b - 1; a <= n - 1 && area(n - 1) < m; --n);
	return n;
}

int main(void){
	printf("%"PRIu32"\n", find_n(.001));
}

