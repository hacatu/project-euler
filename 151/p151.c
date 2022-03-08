#include <stdio.h>

typedef struct{long double a[5];} A;

long double f(A a){
	long double s = 0;
	int j = -1;
	for(int i = 0; i < 5; ++i){
		if(a.a[i]){
			s += a.a[i];
			if(j == -1){
				j = i;
			}
		}
	}
	if(!s){
		return 0;
	}
	if(a.a[j] == s){
		--a.a[j];
		for(int i = j + 1; i < 5; ++i){
			++a.a[i];
		}
		if(!a.a[j]){
			return 1 + f(a);
		}else{
			return f(a);
		}
	}
	long double e = 0;
	for(int i = 0; i < 5; ++i){
		if(a.a[i]){
			A b = a;
			--b.a[i];
			for(int j = i + 1; j < 5; ++j){
				++b.a[j];
			}
			e += a.a[i]/s*f(b);
		}
	}
	return e;
}

int main(void){
	long double e;
	A a = {{1, 0, 0, 0, 0}};
	#pragma omp parallel
	{
		e = f(a);
	}
	printf("%Lf\n", e - 2);
}

