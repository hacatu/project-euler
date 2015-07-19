//p52.c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BASE 23456
#define MAX_N BASE%10

/*
void printDigits(char *digits){
	for(char i = 0; i < 10; ++i){
		printf("%u:%u ", i, digits[i]);
	}
	puts("");
}
*/

void countDigits(uint64_t x, char *xdigits){
	memset(xdigits, 0, 10*sizeof(char));
	while(x){
		++(xdigits[x%10]);
		x /= 10;
	}
}

int main(){
	char xdigits[10] = {0};
	char ndigits[10] = {0};
	uint64_t power = 1;
	for(char i = 0; i < MAX_N; ++i){
		power *= 10;
	}
	for(uint64_t x = BASE + power/10; x != 0; ++x){
		if(MAX_N*x >= power){
			x = BASE + power;
			power *= 10;
		}
		countDigits(x, xdigits);
		for(char n = 2; n <= MAX_N; ++n){
			countDigits(n*x, ndigits);
			if(memcmp(xdigits, ndigits, sizeof(char[10]))){
				goto next_x;
			}
		}
		printf("solution found: %li\n", x);
		break;
		next_x:
		continue;
	}
}

