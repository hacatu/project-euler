//a = sum from 1 to infinity of x^i*(((1+sqrt(5))/2)^i-((1-sqrt(5))/2)^i)/sqrt(5)
//a=-x/(x^2+x-1)
//x = (sqrt(5a^2+2a+1)-a-1)/(2a)
//x is rational when 5a^2+2a+1 is a perfect square.
//The recurrence relation (thanks oeis) for this is a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3)
#include <stdio.h>
#include <inttypes.h>
#include <time.h>

#define MAX 15

int main(void){
	clock_t start = clock();
	uint64_t a1 = 104, a2 = 15, a3 = 2, c = 3, t;
	while(c < MAX){
		t = 8*a1 - 8*a2 + a3;
		a3 = a2;
		a2 = a1;
		a1 = t;
		++c;
	}
	printf("%"PRIu64" %fs\n", a1, (double)(clock() - start)/CLOCKS_PER_SEC);
}

