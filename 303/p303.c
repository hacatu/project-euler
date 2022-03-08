#include <stdio.h>
#include <stdio.h>
#include <inttypes.h>

#include <crater/bitvec.h>

#define N 1000
uint64_t is_small[(N+63)/64] = {};

uint64_t min_sdc_r(uint64_t n, uint64_t r){
	if(is_small[r/64] & (1ull << (r%64))){
		return 0;
	}
	uint64_t res = -1;
	for(uint64_t k0 = 0; k0 < 10; ++k0){
		if((n*k0 + r)%10 > 2){
			continue;
		}
		uint64_t tmp = 10*min_sdc_r(n, (n*k0 + r)/10) + k0;
		if(tmp < res){
			res = tmp;
		}
	}
	return res;
}

uint64_t min_sdc(uint64_t n){
	uint64_t res = -1;
	for(uint64_t k0 = 1; k0 < 10; ++k0){
		if((n*k0)%10 > 2){
			continue;
		}
		uint64_t tmp = 10*min_sdc_r(n, n*k0/10) + k0;
		if(tmp < res){
			res = tmp;
		}
	}
	return res;
}

void init_is_small(uint64_t n, uint64_t d){
	if(!d){
		is_small[n/64] |= 1ull << (n%64);
	}else for(uint64_t a = 0; a < 3; ++a){
		init_is_small(10*n + a, d - 1);
	}
}

int main(){
	init_is_small(0, 4);
}

