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
	int a = 3;
	//from preliminary runs with python, I found that the answer is 1818 and the leg length for a Pythagorean triple
	//that occurs most with one leg under 1818 and the other less than twice the first is 1680 which occurs 49 times.
	//So I can use an 1819 by 50 array of ints.
	int legs[1819][50] = {{0}};
	for(int m = 1; m <= 1818; ++m){
		for(int a, b, n = m&1?2:1; n < m; n += 2){
			if(gcd(m, n) != 1){
				continue;
			}
			a = m*m - n*n;
			b = 2*m*n;
			for(int k = 1;; ++k){
				if(k*a > 1818 && k*b > 1818){
					break;
				}
				if(k*a <= 1818 && b < 2*a){
					legs[k*a][++legs[k*a][0]] = k*b;
				}
				if(k*b <= 1818 && a < 2*b){
					legs[k*b][++legs[k*b][0]] = k*a;
				}
			}
		}
	}
	printf("All triplets calculated\n");
	for(int t = 0; t <= MIN; ++a){
		for(int b, i = 1; i <= legs[a][0]; ++i){
			b = legs[a][i];
			//printf("%i\n", b);
			t += validBoxes(a, b);
		}
		//printf("%i: %i\n", a, t);
	}
	printf("%i\n", --a);
}

