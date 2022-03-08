#include <stdio.h>

#define M 1000000

int main(void){
	int c = 0;
	for(int o = 1, p;; ++o){
		p = (M>>2)/o - o;
		if(p <= 0){
			break;
		}
		c += p;
	}
	printf("%d\n", c);
}

