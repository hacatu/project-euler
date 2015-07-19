#include <stdio.h>

#define TARGET 2000000

int difference(int a, int b){
	if(a > b){
		return a - b;
	}
	return b - a;
}

int main(){
	int min_d = TARGET, min_p;
	for(int a = 1, i = 1;; a += ++i){
		for(int b = 1, j = 1;; b += ++j){
			if(difference(a*b, TARGET) < min_d){
				min_d = difference(a*b, TARGET);
				min_p = i*j;
			}
			if(a*b > TARGET){
				break;
			}
		}
		if(a > TARGET){
			break;
		}
	}
	printf("%i\n", min_p);
}

