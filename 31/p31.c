#include <stdio.h>

int cs(const int *C, int n, int t){
	if(t == 0){
		return 0;
	}
	int s = 0;
	for(int i = 0; i < n; ++i){
		if(C[i] > t){
			break;
		}
		if(C[i] == t){
			++s;
			break;
		}
		s += cs(C + i, n - i, t - C[i]);
	}
	return s;
}

int main(){
	int C[] = {1, 2, 5, 10, 20, 50, 100, 200};
	printf("%i\n", cs(C, 8, 200));
}
