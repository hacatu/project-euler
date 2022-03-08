#include <stdio.h>

int main(void){
	int L[250001] = {0};
	for(int s = 3; s <= 250001; ++s){
		for(int l = 1; 2*l < s; ++l){
			int t = (s - l)*l;
			if(t > 250000){
				break;
			}
			++L[t];
		}
	}
	int N = 0;
	for(int t = 2; t <= 250000; ++t){
		if(1 <= L[t] && L[t] <= 10){
			++N;
		}
	}
	printf("%i\n", N);
}

