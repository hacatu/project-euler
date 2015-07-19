#include <stdio.h>
#include <stdint.h>

typedef unsigned __int128 uint128_t;

uint128_t flip(uint128_t n){
	uint128_t m = 0;
	while(n){
		m *= 10;
		m += n%10;
		n /= 10;
	}
	return m;
}

char isLychrel(uint128_t n){
	uint128_t m;
	m = flip(n);
	n += m;
	//printf("%llu\n", n);
	for(int i = 1; i < 50; ++i){
		m = flip(n);
		if(m == n){
			return 0;
		}
		n += m;
		//printf("%llu\n", n);
	}
	return 1;
}

int main(){
	int c = 0;
	for(int i = 1; i < 10000; ++i){
		if(isLychrel(i)){
			++c;
		}
	}
	printf("%i\n", c);
	//isLychrel(4994);
}

