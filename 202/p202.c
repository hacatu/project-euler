#include <stdio.h>
#include <inttypes.h>

#define REFLECTIONS 12017639147
//#define REFLECTIONS    1000001

uint64_t gcd_r(uint64_t x, uint64_t y){
	if(!y){
		return x;
	}
	return gcd_r(y, x%y);
}

uint64_t gcd(uint64_t x, uint64_t y){
	uint64_t p2 = __builtin_ctz(x|y);
	if(x < y){
		return (1 << p2)*gcd_r(y >> p2, x >> p2);
	}
	return (1 << p2)*gcd_r(x >> p2, y >> p2);
}

int main(void){
	if(REFLECTIONS == 1){
		puts("1");
		return 0;
	}
	uint64_t x = (REFLECTIONS + 1) >> 1, y = 1, ways = 0;
	if(x%3 == 2){
		x -= 2, y += 2;
	}else if(x%3 == 0){
		x -= 1, y += 1;
	}
	printf("(%"PRIu64", %"PRIu64")\n", x, y);
	for(; x > y; x -= 3, y += 3){
		if(gcd(x, y) == 1){
			++ways;
		}
	}
	ways <<= 1;
	printf("%"PRIu64"\n", ways);
}

