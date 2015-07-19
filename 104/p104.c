#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#define R_5 sqrtl(5.)
#define LOG_R_5 log10l(R_5)
#define PHI (1.+R_5)/2.
#define LOG_PHI log10l(PHI)

uint64_t pdtlMod[283697];//expected: 544320 = 9!/10^9*1500000000 (the period of F(n)%10^9), actual: 283697
uint64_t pdtlL = 0;

int isPandigital(int a){
	int digitCounts = 1;
	int digit;
	if(a%9){
		return 0;
	}
	while(a){
		digit = 1<<(a%10);
		if(digit & digitCounts){
			return 0;
		}
		digitCounts |= digit;
		a /= 10;
	}
	return digitCounts == 0b1111111111;
}

int isPdtlStartF(uint64_t k){
	long double f = LOG_PHI*(long double)(k) - LOG_R_5;
	f = fmodl(f, 1);//get fractional part
	f = powl(10, f + 8);
	return isPandigital((int)f);
}

void FMStep(uint64_t *a, uint64_t *b){
	uint64_t t = *a;
	*a += *b;
	*b = t;
	*a %= 1000000000;
}

int main(void){
	for(uint64_t n = 1, a = 1, b = 0; a; FMStep(&a, &b), ++n){
		if(isPandigital(a)){
			pdtlMod[pdtlL++] = n;
			if(isPdtlStartF(n)){
				printf("%llu\n", (unsigned long long)n);
				return 0;
			}
		}
	}
	for(uint64_t n = 1500000000; ; n += 1500000000){//period of F(n)%10^9)
		printf("%llu\n", (unsigned long long)n);
		for(uint64_t m = 0; m < pdtlL; ++m){
			if(isPdtlStartF(n + pdtlMod[m])){
				printf("%llu\n", (unsigned long long)(n + pdtlMod[m]));
				return 0;
			}
		}
	}
}

