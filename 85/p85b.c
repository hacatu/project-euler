#include <stdio.h>
#include <math.h>

#define TARGET 2000000

int difference(int a, int b){
	if(a > b){
		return a - b;
	}
	return b - a;
}

int main(){
	int min_d = TARGET, min_p, j, b;
	for(int a = 1, i = 1; a < TARGET; a += ++i){
		j = (sqrt(1. + 8.*(double)TARGET/(double)a) - 1.)/2.;
		b = j*(j+1)/2;
		if(difference(a*b, TARGET) < min_d){
			min_d = difference(a*b, TARGET);
			min_p = i*j;
		}
		b += j + 1;
		if(difference(a*b, TARGET) < min_d){
			min_d = difference(a*b, TARGET);
			min_p = i*j;
		}
	}
	printf("%i\n", min_p);
}

