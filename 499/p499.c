#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double p(double m, double s, double ppn){
	double l = 2*m > s ? log(2*m-s)/log(2) : 0, ret = 0;
	for(double n = ceil(l), pn = pow(.5, n); ppn*pn > 1e4; pn = pow(.5, ++n)){
		//printf("%f\n", ppn*pn);
		ret += p(m, pow(2, n) + s - m, ppn*pn)*pn;
	}
	return ret ? ret : 1;
}

int main(void){
	double pf = p(2, 2, 1e7);
	printf("%f\n", pf);
}

