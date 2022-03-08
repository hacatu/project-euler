#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000000

#define MAXFACTS 7

typedef struct{int factor, power;} Fact;

typedef Fact Facts[MAXFACTS];

static inline void mark(Facts facts, int m, int p){
	int i;
	for(i = 0; i < MAXFACTS; ++i){
		if(facts[i].factor){
			continue;
		}
		facts[i].factor = p;
		while(!(m%p)){
			m /= p;
			++facts[i].power;
		}
		break;
	}
}

void fsieve(int max, Facts facts[max]){
	memset(facts, 0, max*sizeof(Facts));
	for(int p = 2; p < max; ++p){
		if(facts[p][0].factor){
			continue;
		}
		facts[p][0].factor = p;
		facts[p][0].power = 1;
		for(int m = 2*p; m < max; m += p){
			mark(facts[m], m,  p);
		}
	}
}

int dr(int n){
	return n - (n - 1)/9*9;
}

int mdrsr(int n, int f, int m, int i, Fact *facts, int *mdrs){
	if(i == MAXFACTS || !facts[i].factor){
		if(f == 1){
			return mdrs[n*f] = dr(n);
		}
		i = mdrs[f] + mdrs[n];
		return mdrs[n*f] = i > m ? i : m;
	}
	for(int p = 0; p <= facts[i].power && f <= n; ++p){
		m = mdrsr(n, f, m, i + 1, facts, mdrs);
		if(p == facts[i].power){
			break;
		}
		f *= facts[i].factor;
		n /= facts[i].factor;
	}
	return mdrs[n*f] = m;
}

int mdrs(int n, Facts facts, int *mdrs){
	return mdrsr(n, 1, 0, 0, facts, mdrs);
}

int main(void){
	Facts *facts = malloc(MAX*sizeof(Facts));
	int *mdrs_ = malloc(MAX*sizeof(int));
	fsieve(MAX, facts);
	int s = 0;
	for(int n = 2; n < MAX; ++n){
		s += mdrs(n, facts[n], mdrs_);
	}
	printf("%i\n", s);
	free(facts);
	free(mdrs_);
}

