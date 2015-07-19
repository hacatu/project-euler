#include <stdio.h>
#include <math.h>

#define MAX 1500000

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

int triangleCounts[MAX + 1] = {0};

void seiveTriangles(){//use a = m**2-n**2, b = 2*m*n, c = m**2 + n**2 if one of m, n is odd and n and m are coprime
	for(int m = 1; m < sqrt(MAX/2); ++m){
		for(int n = m&1?2:1, p; n < m && n <= (MAX/2)/m - m; n += 2){//n is even if m is odd and odd if m is even
			if(gcd(m, n) != 1){
				continue;
			}
			p = (m<<1)*(m + n);//set l to the perimeter and ignore the individual side lengths a, b, and c
			for(int l = p; l <= MAX; l += p){
				++triangleCounts[l];
			}
		}
	}
}

int main(){
	seiveTriangles();
	int c = 0;
	for(int l = 12; l <= MAX; ++l){
		if(triangleCounts[l] == 1){
			++c;
		}
	}
	printf("%i\n", c);
}

