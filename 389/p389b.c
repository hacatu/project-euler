#include <stdio.h>
#include <inttypes.h>

int main(){
	uint64_t sides[5] = {4, 6, 8, 12, 20};
	double E = 1, V = 0;
	for(size_t i = 0; i < 5; ++i){
		V = V*(sides[i] + 1)*(sides[i] + 1)/4 + E*(sides[i]*sides[i] - 1)/12;
		E = E*(sides[i] + 1)/2;
	}
	printf("%f\n", V);
}

