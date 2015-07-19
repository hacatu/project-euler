#include <stdio.h>

#define MIN 100
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

int validBoxes(int M, int a, int b){
	return b/2 - MAX(1, b - a) + 1;
}

int countBoxes(int M, int a, int b){
	if(a > M || b > 2*a){//invalid box
		return 0;
	}
	//printf("%i, %i\n", a, b);
	int t = 0;
	for(int i = 1; i <= M/a; ++i){
		t += validBoxes(M, a*i, b*i);
	}
	return t;
}

int main(){
	int M = 4;
	for(int t = 0; t <= MIN; ++M){
		t = 0;
		for(int m = 1; m <= M; ++m){
			//printf("m: %i\n", m);
			for(int a, b, n = m&1?2:1; n < m; n += 2){//n is even if m is odd and odd if m is even
				//printf("n: %i\n", n);
				if(gcd(m, n) != 1){
					continue;
				}
				a = m*m - n*n;
				b = 2*m*n;
				t += countBoxes(M, a, b) + countBoxes(M, b, a);
			}
		}
		printf("%i: %i\n", M, t);
	}
	printf("%i\n", M);
}

