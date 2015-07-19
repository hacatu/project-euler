#include <stdio.h>

#define MIN 1000000
#define MAX(a, b) ((a)>(b)?(a):(b))

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

int validBoxes(int a, int b){
	return b/2 - MAX(1, b - a) + 1;
}

int main(){
	int M = 3;
	for(int t = 0; t <= MIN; ++M){
		for(int m = 1; m <= M; ++m){
			//printf("m: %i\n", m);
			for(int a, b, n = m&1?2:1; n < m; n += 2){//n is even if m is odd and odd if m is even
				//printf("n: %i\n", n);
				if(gcd(m, n) != 1){
					continue;
				}
				a = m*m - n*n;
				b = 2*m*n;
				if(M%a == 0 && b <= 2*a){
					t += validBoxes(M, b*M/a);
				}
				if(M%b == 0 && a <= 2*b){
					t += validBoxes(M, a*M/b);
				}
			}
		}
		printf("%i: %i\n", M, t);
	}
	printf("%i\n", M);
}

