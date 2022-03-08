#include <inttypes.h>
#include <stdio.h>

const uint64_t DMR_PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
const uint64_t DMR_PRIMES_C = 9;
const uint64_t N = 50000000ull;
//const uint64_t N = 10000ull;

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

int isPrime(uint64_t n){
	uint64_t s, d;//s, d | 2^s*d = n - 1
	if(n%2 == 0){
		return n == 2;
	}
	--n;
	s = __builtin_ctz(n);
	d = n>>s;
	++n;
	for(uint64_t i = 0, a, x; i < DMR_PRIMES_C; ++i){
		a = DMR_PRIMES[i];
		if(a >= n){
			break;
		}
		x = powmod(a, d, n);
		if(x == 1 || x == n - 1){
			goto CONTINUE_WITNESSLOOP;
		}
		for(a = 0; a < s - 1; ++a){
			x = powmod(x, 2, n);
			if(x == 1){
				return 0;
			}
			if(x == n - 1){
				goto CONTINUE_WITNESSLOOP;
			}
		}
		return 0;
		CONTINUE_WITNESSLOOP:;
	}
	return 1;
}

int main(){
	uint64_t c = 1;//7 is skipped
	#pragma omp parallel for reduction(+:c)
	for(uint64_t n = 2; n <= N; ++n){
		if(n%7 == 2 || n%7 == 5){
			continue;
		}
		if(isPrime(2*n*n-1)){
			++c;
		}
	}
	printf("%"PRIu64"\n", c);
}

