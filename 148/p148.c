#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

/* Looking at Pascal's triangle mod 7, it becomes clear that after 7 rows there are 2
 * more triangles starting at 1 and 1, then after 7 more at 1, 2, and 1, then 3 more
 * starting at 1, 3, 3, and 1, and you can see the pattern.  It is self similar like the serpinski triangle,
 * except there are 7 layers of triangles instead of 2 per unit.  A formula for the number of nonzero cells is as follows:
 * f(n) = T(n) if n <= 7
 * f(n) = T(floor(n/7^i))*28^i + ceil(n/7^i)*f(n%7^i) where i = ceil(log(7, n)) - 1 otherwise
 * where T(n) = n*(n + 1)/2
 */

uint64_t T(uint64_t n){
	return n*(n + 1)/2;
}

uint64_t f(uint64_t n){
	if(n <= 7){
		return T(n);
	}else{
		double i = ceil(log(n)/log(7)) - 1;
		return T(floor(n/pow(7, i)))*pow(28, i) + ceil(n/pow(7, i))*f(n%(uint64_t)pow(7, i));
	}
}

int main(void){
	clock_t start = clock();
	printf("%"PRIu64" %9fs\n", f(1000000000), (double)(clock() - start)/(CLOCKS_PER_SEC));
}

