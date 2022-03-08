#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

uint64_t find_latticepts_diag(uint64_t n){
	uint64_t ret = 2;
	printf("(0,±%"PRIu64")", 2*n);
	/* 4*n*n = 3*x*x + y*y
	 * dy/dx = -3x/y
	 * x_(i+1) = x_i + 1
	 * y_(i+1) <= y_i - 3*x_i/y_i
	 */
	uint64_t x = ceil(sqrt((4*n + 1)/3)), y = 2*n - 1;
	/* For the first part of the ellipse, the y coordinate decreases slower than the x coordinate increases
	 * Thus for y to decrease by 1, x must increase by more than 1
	 * The changeover point is of course when dy/dx = -1 = -3x/y
	 * y = 3x ie x = n/sqrt(3), y = n*sqrt(3)
	 */
	while(y*y > 3*n*n){//we start with (x, y) as an inclusive exterior point
		while(3*x*x + y*y > 4*n*n){
			--x;
		}
		if(3*x*x + y*y == 4*n*n){
			printf(",(±%"PRIu64",±%"PRIu64")", x, y);
			ret += 4;
		}else{
			++x;
		}//(x, y) is an inclusive exterior point
		--y;
		x += (y + 3*x)/(3*x);//now (x, y) is a strict exterior point
	}
	while(1){//(x, y) is an inclusive exterior point
		y -= 3*x/y;
		++x;//(x, y) is a strict exterior point
		if(3*x*x > 4*n*n){
			break;
		}
		while(3*x*x + y*y > 4*n*n){
			--y;
		}
		if(3*x*x + y*y == 4*n*n){
			printf(",(±%"PRIu64",±%"PRIu64")", x, y);
			ret += 4;
		}else{
			++y;
		}//(x, y) is an inclusive interior point
	}
	return ret;
}

uint64_t find_latticepts_orth(uint64_t n){
	return 0;
}

int main(){
	printf("n  |\n");
	for(uint64_t n = 1; n < 1000; ++n){
		printf("%.3"PRIu64"|", n);
		find_latticepts_diag(n);
		printf("\n");
	}
}

