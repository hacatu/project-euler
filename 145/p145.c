#include <stdio.h>
#include <inttypes.h>

#define MAX 1000000000

uint64_t reverse2[100], reverse3[1000];

uint64_t oddl[1000], oddg[1000];

void init(void){
	for(uint64_t n = 0; n < 10; ++n){
		reverse2[n] = 10*n;
		reverse3[n] = 100*n;
		oddl[n] = (n&1);
		oddg[n] = 0;
	}
	for(uint64_t n = 10; n < 100; ++n){
		reverse2[n] = n%10*10 + n/10;
		reverse3[n] = 10*reverse2[n];
		oddl[n] = ((n/10)&1) && (n&1);
		oddg[n] = 0;
	}
	for(uint64_t n = 100; n < 1000; ++n){
		reverse3[n] = n%10*99 + n%100 + n/100;
		oddg[n] = oddl[n] = oddl[n/10] && (n&1);
	}
}

int odd(uint64_t n){
	while(n >= 1000){
		if(!oddg[n%1000]){
			return 0;
		}
		n /= 1000;
	}
	return oddl[n];
}

uint64_t reverset(uint64_t n){
	if(n < 100){
		return reverse2[n];
	}
	if(n < 1000){//good
		return reverse3[n];
	}
	if(n < 10000){//good
		return reverse3[n/10] + n%10*1000;
	}
	if(n < 100000){
		return reverse3[n/100] + reverse2[n%100]*1000;
	}
	if(n < 1000000){
		return reverse3[n/1000] + reverse3[n%1000]*1000;
	}
	if(n < 10000000){
		return reverse3[n/10000] + reverse3[n%1000]*10000 + n/1000%10*1000;
	}
	if(n < 100000000){
		return reverse3[n/100000] + reverse3[n%1000]*100000 + reverse2[n/1000%100]*1000;
	}
	return reverse3[n/1000000] + reverse3[n%1000]*1000000 + reverse3[n/1000%1000]*1000;
}

int reversible(uint64_t n){
	if(n < 10){
		return 0;
	}
	if(!(n%10)){
		return 0;
	}
	return odd(n + reverset(n));
}

int main(void){
	init();
	int c = 0;
	#pragma omp parallel for reduction(+:c)
	for(uint64_t n = 1; n < MAX; ++n){
		if(reversible(n)){
			++c;
		}
	}
	printf("%i\n", c);
}

