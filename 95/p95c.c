#include <stdio.h>

int next[] =   {1, 2, 1}, cyclens[] = {0, 0, 0}, depth = -1;

int cyclen(int n){
	int len;
	if(cyclens[n] == 0){
		cyclens[n] = --depth;
		len = cyclen(next[n]);
		if(depth == -1)return -1;
		if(depth >= cyclens[n])return cyclens[n] = len;
		return cyclens[n] = -1;
	}else if(cyclens[n] < -1){
		len = cyclens[n] -depth + 1;
		depth = cyclens[n];
		return len;
	}
	depth = -1;
	return cyclens[n];
}

int main(){
	cyclen(1);
	printf("%i\n", cyclen(0));
}

