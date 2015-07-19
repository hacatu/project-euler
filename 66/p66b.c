#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

#define SCALE 2

struct{
	uint64_t a0;
	uint64_t *a;
	uint64_t max;
	uint64_t len;
} a;

uint64_t a_(uint64_t i){
	if(!i){
		return a.a0;
	}
	return a.a[(i - 1)%a.len];
}

void printa(){
	printf("a.len: %" PRIu64 "\na.max: %" PRIu64 "\na: [%" PRIu64 "; (", a.len, a.max, a.a0);
	for(uint64_t i = 1; i < a.len; ++i){
		printf("%" PRIu64 ", ", a_(i));
	}
	printf("%" PRIu64 ")]\n", a_(a.len));
}

char isSquare(uint64_t n){
	uint64_t r = sqrt(n);
	return n == r*r ? 1 : 0;
}

void addCoef(uint64_t c){
	while(a.len + 1 >= a.max){
		uint64_t *temp = realloc(a.a, SCALE*a.max*sizeof(uint64_t));
		if(!temp){
			return;
		}
		a.a = temp;
		a.max *= SCALE;
	}
	a.a[a.len++] = c;
}

uint64_t coef(uint64_t r, uint64_t n, uint64_t d){
	return n/(sqrt(r) - d);
}

//d should not be a perfect square
void computePFrac(uint64_t r){
	uint64_t n0, d0, n = r, d = 0, c = coef(r, n, d);
	a.a0 = c;
	a.len = 0;
	n0 = n = (r - d*d)/n;
	d0 = d = c*n - d;
	do{
		c = coef(r, n, d);
		addCoef(c);
		n = (r - d*d)/n;
		d = c*n - d;
	}while(n != n0 || d != d0);
	a.len;
}

char isPellSolution(uint64_t x, uint64_t d, uint64_t y){
	if((x*x - 1)%d){
		return 0;
	}
	if((x*x - 1)/d == y*y){
		return 1;
	}
	return 0;
}

uint64_t findMinPellSolution(uint64_t d){
	computePFrac(d);
	for(uint64_t x1 = 0, y1 = 1, x = 1, y = 0, i = 0, temp; x > x1; ++i){// x <= x1 implies overflow.
		temp = x;
		x = a_(i)*x + x1;
		x1 = temp;
		temp = y;
		y = a_(i)*y + y1;
		y1 = temp;
		if(isPellSolution(x, d, y)){
			return x;
		}
	}
	return 0;//overflow occured
}

int main(){
	uint64_t maxMinX = 0, maxMinD = 0;
	a.a0 = 0;
	a.a = malloc(1*sizeof(uint64_t));
	if(!a.a){
		return 1;
	}
	a.max = 1;
	a.len = 0;
	for(uint64_t d = 2, minX; d <= 1000; ++d){
		if(isSquare(d)){
			continue;
		}
		minX = findMinPellSolution(d);
		if(!minX){
			puts("overflow");
		}
		//printf("%" PRIu64 "^2 - %" PRIu64 "*%" PRIu64 "^2 = 1\n", minX, d, (uint64_t)sqrt(y2));
		if(minX > maxMinX){
			maxMinX = minX;
			maxMinD = d;
			printf("New maxMinD: %" PRIu64 "\n", maxMinD);
		}
	}
	printf("%" PRIu64 "\n", maxMinD);
	free(a.a);
}

