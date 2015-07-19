#include <stdio.h>
#include <math.h>

int main(){
	FILE *file = fopen("p99.txt", "r");
	if(!file){
		return 1;
	}
	double base, exp, max = 0;
	int max_l = 0;
	for(int i = 1; i <= 1000; ++i){
		if(fscanf(file, "%lf,%lf", &base, &exp) < 2){
			break;
		}
		exp *= log2(base);
		if(exp > max){
			max = exp;
			max_l = i;
		}
	}
	printf("%i\n", max_l);
}

