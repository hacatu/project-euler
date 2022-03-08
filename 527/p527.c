#include <stdio.h>
#include <math.h>
#include <limits.h>

#define N 1e10

#define M_EULER 0.57721566490153286060651209008240243
/*
double rr(int n){
	double s = 0;
	for(int i = 1; i < n; ++i){
		s += i*rr(i);
	}
	return 1 + s*2/(n*n);
}
*//*
double ri(int n){
	if(!n){
		return 0;
	}
	double r = 1;
	for(int i = 2; i < n; ++i){
		r = i + r*(i + 2)/i;
	}
	return 1 + r*2/(n*n);
}
*/
double re(double n){
	return 1 + 2.*(n + 1)/n*(log(n + 1) + M_EULER -2 + 1/(n + 1));
}
/*
double br(int n){
	if(!n){
		return 0;
	}
	int i = (n + 1)/2;
	return 1 + ((i - 1)*br(i - 1) + (n - i)*br(n - i))/n;
}
*//*
double bi(int n){
	double s = 0;
	int p = sizeof(n)*CHAR_BIT - __builtin_clz(n + 1) - 1;
	for(int i = 1; i <= p; ++i){
		s += (double)i*(1<<(i - 1));
	}
	return (s + (p + 1)*(n - (1<<p) + 1))/n;
}
*/
double be(double n){
	double p = floor(log(n + 1)/log(2));
	return ((p + 1)*(n + 1) + 1 - (pow(2, p + 1)))/n;
}

int main(void){
	printf("%.8f\n", re(N) - be(N));
}

