#include <inttypes.h>
#include <stdio.h>

const uint64_t N = 10;

static inline uint64_t generate_m(uint64_t a, uint64_t b){
	uint64_t count = 0;
	for(uint64_t m = 1, A = a, B = b; A + 2*B <= N; ++m, A += a, B += b){
		//count += # C with B <= C <= A + B - 1 and gcd(C, m) == 1
	}
}

static uint64_t generate_ab_farey(){
	uint64_t n = N/2 - 1;
	uint64_t a = 0, b = 1, c = 1, d = n;
	uint64_t count = 0;
	while(1){
		if(a + b <= N/2){
			count += generate_m(a, b);
		}
		if(c == 1 && d == 1){
			break;
		}
		uint64_t k = (n + b)/d;
		uint64_t p = k*c - a, q = k*d - b;
		a = c, b = d;
		c = p, d = q;
	}
	return count;
}

int main(){
	generate_ab_farey();
}

