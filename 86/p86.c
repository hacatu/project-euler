#include <stdio.h>
#include <math.h>

#define MIN 1000000

int isPerfectSquare(int n){
	return sqrt(n) == (int)sqrt(n);
}

int main(){
	int M = 1818;
	for(int t = 0; t < MIN; ++M){
		t = 0;
		for(int a = 3; a <= M; ++a){
			for(int b = 1; b <= a; ++b){
				for(int c = 1; c <= b; ++c){
					if(isPerfectSquare(a*a + (b + c)*(b + c))){
						++t;
					}
				}
			}
		}
		printf("%i: %i\n", M, t);
	}
	printf("%i\n", M);
}

