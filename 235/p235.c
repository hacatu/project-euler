#include <stdio.h>
#include <math.h>

static inline long double f(long double x){
	return -4700.L*powl(x, 5001.L) + 4701.L*powl(x, 5000.L) + 2e11L*x*x - 400000000300.L*x + 200000000299.L;
}

static inline long double f_(long double x){
	return -23504700.L*powl(x, 5000.L) + 23505000.L*powl(x, 5000.L) + 4e11L*x - 400000000300.L;
}

int main(void){
	long double x = 1.002322L;
	for(int i = 0; i < 12; ++i){
		printf("%Lf\n", x -= f(x)/f_(x));
	}
}