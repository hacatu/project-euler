#include <stdio.h>
#include <inttypes.h>

#define LEN 50
#define COLORS 3

uint64_t _cb[COLORS][LEN] = {{[0 ... 1] = 1, [2] = 2}, {[0 ... 2] = 1, [3] = 2}, {[0 ... 3] = 1, [4] = 2}};

uint64_t cb(uint64_t n, uint64_t m){
	if(_cb[m - 2][n]){
		return _cb[m - 2][n];
	}
	return _cb[m - 2][n] = cb(n - 1, m) + cb(n - m, m);
}

uint64_t b(uint64_t n){
	return cb(n, 2) + cb(n, 3) + cb(n, 4) - 3;
}

int main(void){
	
}

