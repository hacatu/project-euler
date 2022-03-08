#include <stdio.h>
#include <inttypes.h>

int main(void){
	uint64_t r = 101010103;
	for(uint64_t s, d;;){
		s = r*r/100;
		d = 8;
		while(s){
			if(s%10 != d--)break;
			s /= 100;
		}
		if(!s)break;
		r += 4;
		s = r*r/100;
		d = 8;
		while(s){
			if(s%10 != d--)break;
			s /= 100;
		}
		if(!s)break;
		r += 6;
	}
	printf("%"PRIu64"\n", r*10);
}

