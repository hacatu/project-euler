#include <stdio.h>

#define MAX 9999
#define MIN 1000
#define MIN_STEP 45
//The first triangular number with 4 digits is the 45th.
//The step from the previous triangular number to it is 45.
//This is the smallest step to any first figurate number with 4 digits.

typedef struct f8{
	unsigned char head;
	unsigned char tail;
	unsigned short number;
} f8;

inline void addf8(int n, f8 f8s[MAX/MIN_STEP]){
	f8s[n/MIN_STEP] = (f8){.head = n/100, .tail = n%100, .number = n};
}

inline char hasChain(int n, f8 is_f8[5][MAX/MIN_STEP]){
	
}

int main(){
	f8 is_f8[5][MAX/MIN_STEP] = {{{0}}};
	for(int p = 0; p < 5; ++p){
		for(int g = 1, n = 1; n <= MAX; g += p + 1, n += g){
			if(n < MIN){
				continue;
			}
			addf8(n, is_f8[p]);
		}
	}
	for(int g = 1, n = 1; n <= MAX; g += 6, n += g){
		if(n < MIN){
			continue;
		}
		if(hasChain(n, is_f8)){
			return 0;
		}
	}
}

