#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPS 0.000000001
#define MIN 2
#define MAX 45

int f(double s, double *iss, double *rss, size_t l){
	if(fabs(s) < EPS){
		return 1;
	}
	int c = 0;
	for(size_t i = 0; i < l; ++i){
		if(s > rss[i] + EPS){
			break;
		}
		if(s > rss[i] - EPS){
			++c;
			break;
		}
		if(s < iss[i] - EPS){
			continue;
		}
		c += f(s - iss[i], iss + i, rss + i, l - i);
	}
	return c;
}

int main(void){
	double iss[MAX - MIN + 1];
	double rss[MAX - MIN + 1];
	for(size_t i = 0; i < MAX - MIN + 1; ++i){
		iss[i] = 1./(double)(i + MIN)/(double)(i + MIN);
	}
	rss[MAX - MIN] = iss[MAX - MIN];
	for(int i = MAX - MIN - 1; i >= 0; --i){
		rss[i] = rss[i + 1] + iss[i];
	}
	int c;
	#pragma omp parallel
	{
		c = f(0.5, iss, rss, MAX - MIN + 1);
	}
	printf("%d\n", c);
}

