#include <stdio.h>
#include <inttypes.h>
#include <hac_avl.h>

#define MAX 100000000

uint64_t reverse[1000];

void init(void){
	for(uint64_t n = 0; n < 10; ++n){
		reverse[n] = n;
	}
	for(uint64_t n = 10; n < 100; ++n){
		reverse[n] = n%10*10 + n/10;
	}
	for(uint64_t n = 100; n < 1000; ++n){
		reverse[n] = n%10*99 + n%100 + n/100;
	}
}

int isPalindrome(uint64_t n){
	if(n < 10){
		return 1;
	}
	if(!(n%10)){
		return 0;
	}
	if(n < 100){
		return !(n%11);
	}
	if(n < 1000){
		return n%10 == n/100;
	}
	if(n < 10000){
		return n%100 == reverse[n/100];
	}
	if(n < 100000){
		return n%100 == reverse[n/1000];
	}
	if(n < 1000000){
		return n%1000 == reverse[n/1000];
	}
	if(n < 10000000){
		return n%1000 == reverse[n/10000];
	}
	if(n%1000 != reverse[n/100000]){
		return 0;
	}
	return !(n/1000%100%11);
}

int cmpUint64s(uint64_t a, uint64_t b){
	if(a > b){
		return -1;
	}else if(a < b){
		return 1;
	}
	return 0;
}

int main(){
	HAC_AVL_T_NEW(uint64_t);
	HAC_AVL_T(uint64_t) P = HAC_AVL_EMPTY(uint64_t);
	init();
	uint64_t S = 0, c = 0;
	for(uint64_t a = 1, s; ; ++a){
		s = a*a;
		if(2*(s + a) + 1 >= MAX){
			break;
		}
		for(uint64_t b = a + 1; ; ++b){
			s += b*b;
			if(s >= MAX){
				break;
			}
			if(isPalindrome(s)){
				if(HAC_AVL_INSERT(uint64_t, &P, s, cmpUint64s(e1, e2))){
					S += s;
				}
			}
		}
	}
	HAC_AVL_DELETE(uint64_t, &P);
	printf("%"PRIu64"\n", S);
}

