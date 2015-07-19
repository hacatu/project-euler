//For a recurrence relation a(n)=c(1)*a(n-1)+c(2)*a(n-2)+...+c(d)*a(n-d) where a(1)...a(d) are predefined, the generating function is:
//a=(a(1)*x+(a(2)-a(1))*x^2+...+(a(d)-a(d-1))*x^d)/(1-(c(1)*x+...+c(d)*x^d))
//a=(x+3*x^2)/(1-x-x^2)
//5*a^2+14*a+1 is a perfect square
//Output (note this has overflow and so is wrong after 20 or so):
/*
2
5
21
42
152
296
1050
2037
7205
13970
49392
95760
338546
656357
2320437
4498746
15904520
30834872
109011210
211345365
747173957
1448582690
1939563256
3440811918
4705091084
5139146800
5361128808
9815853856
12847310053
14281773925
60101516332
 */
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#define MAX 30

int main(void){
	uint64_t c = 0, s = 0, a = 1, b, r;
	while(c < MAX){
		b = 5*a*a + 14*a + 1;
		r = sqrt(b);
		if(b == r*r){
			printf("%"PRIu64"\n", a);
			++c;
			s += a;
		}
		++a;
	}
	printf("%"PRIu64"\n", s);
}

