#include <stdio.h>
#include <math.h>

#define M_E 2.71828182845904523536028747135266249775724709369995

#define MAX 10000

int main(void){
	int s = 0;
	for(int N = 5, k; N <= MAX; ++N){
		k = round(N/M_E);
		/*{
			long double k_f = floorl(N/M_E), k_c = ceill(N/M_E);
			k = (powl(N/k_f, k_f) > powl(N/k_c, k_c)) ? k_f : k_c;
		}*/
		while((k&1) == 0){
			k >>= 1;
		}
		while(k%5 == 0){
			k /= 5;
		}
		if(N%k){
			s += N;
		}else{
			s -= N;
		}
	}
	printf("%i\n", s);
}

