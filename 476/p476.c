#define _GNU_SOURCE
#include <inttypes.h>
#include <stdio.h>
#include <math.h>

const uint64_t N = 1803;

double R(double a, double b, double c){
	double s = (a + b + c)/2.;
	double A = sqrt(s*(s - a)*(s - b)*(s - c));
	double ri = A/s;
	double ha = sqrt((s - a)*(s - a) + ri*ri);
	double hb = sqrt((s - b)*(s - b) + ri*ri);
	double ra = ri*(ha - ri)/(ha + ri);
	double rb = ri*(hb - ri)/(hb + ri);
	double rd = ra*ra/ri;
	if(rd > rb){
		return M_PI*(ri*ri + ra*ra + rd*rd);
	}else{
		return M_PI*(ri*ri + ra*ra + rb*rb);
	}
}

int main(){
	double net = 0;
	uint64_t num = 0;
	#pragma omp parallel for reduction(+:net,num)
	for(uint64_t a = 1; a <= N/2; ++a){
		for(uint64_t b = a; b <= N - a; ++b){
			for(uint64_t c = b; c < a + b; ++c){
				double A = R(a, b, c);
				net += A;
				++num;
			}
		}
	}
	printf("%f\n", net/num);
}

