#include <stdio.h>
#include <inttypes.h>

#define MAX 10
#define MOD 100000
#define P 1

uint64_t powmod(unsigned __int128 b, uint64_t e, uint64_t n){
	unsigned __int128 r = 1;
	b %= n;
	while(e){
		if(e&1){
			r = r*b%n;
		}
		e >>= 1;
		b = b*b%n;
	}
	return (uint64_t)r;
}

unsigned __int128 fpow(unsigned __int128 n, unsigned __int128 f, unsigned __int128 g){
	unsigned __int128 r = 1;
	while(!(n%f)){
		n /= f;
		r *= g;
	}
	return r;
}

int main(void){
	unsigned __int128 f = 1*2*3*4, t = (MAX - 1)/5;
	#pragma omp parallel for reduction(*:f)
	for(unsigned __int128 i = 5; i < MOD; i += 5){
		f *= i*(i + 1)*(i + 2)*(i + 3)*(i + 4)/4/fpow(i, 5, 5);
		f %= MOD;
	}
	f %= MOD;
	f *= MOD;
	f /= fpow(MOD, 5, 10);
	f %= MOD;
	printf("%"PRIu64"\n", (uint64_t)f);
	f = powmod(f, P, MOD);
	for(unsigned __int128 p = 25; p < MAX; p *= 5){
		t -= (MAX - 1)/p;
	}//t is the excess number of 2s removed
	f *= powmod(2, t, MOD);//note: this could potentially be optimized using bitshifts.
	f %= MOD;
	printf("%"PRIu64"\n", (uint64_t)f);
}

