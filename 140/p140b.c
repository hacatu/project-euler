/* For a recurrence relation a(n)=c(1)*a(n-1)+c(2)*a(n-2)+...+c(d)*a(n-d) where a(1)...a(d) are predefined, the generating function is:
 * a=(a(1)*x+(a(2)-a(1))*x^2+...+(a(d)-a(d-1))*x^d)/(1-(c(1)*x+...+c(d)*x^d))
 * a=(x+3*x^2)/(1-x-x^2)
 * 5*a^2+14*a+1 is a perfect square
 * 0, 2, 5, 21, 42, 152, 296, 1050, 2037, ...
 * a(n) = 7*a(n-2) - a(n-4) + 7
 */
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#define MAX 30

int main(void){
	clock_t start = clock();
	uint64_t a = 42, a1 = 21, a2 = 5, a3 = 2, t, c = 4, s = 42 + 21 + 5 + 2;
	while(c < MAX){
		t = 7*a1 - a3 + 7;
		a3 = a2;
		a2 = a1;
		a1 = a;
		a = t;
		s += a;
		++c;
	}
	printf("%"PRIu64" %fs\n", s, (double)(clock() - start)/CLOCKS_PER_SEC);
}

