#include <stdio.h>
#include <inttypes.h>
#include <primesieve.h>

#define MAX 100000000

int64_t mmi(int64_t a, int64_t b){
	int64_t r0 = b, r1 = a;
	int64_t s0 = 1, s1 = 0;
	int64_t t0 = 0, t1 = 1;
	while(r1){
		int64_t q = r0/r1, t;
		t = r1;
		r1 = r0 - q*r1;
		r0 = t;
		t = s1;
		s1 = s0 - q*s1;
		s0 = t;
		t = t1;
		t1 = t0 - q*t1;
		t0 = t;
	}
	return t0 < 0 ? t0 + b : t0;
}

inline uint64_t S_fact(uint64_t p){
	uint64_t r = 1;
	for(uint64_t i = 1; i <= p - 5; ++i){
		r = r*i%p;
	}
	return r*9%p;
}

inline int64_t S_mmi(int64_t p){
	//(p-1)*... + ... + (p-5)! = ((p-1)*...*(p-4) + ... + 1)*(p-5)!
	//=(p^4 - 9*p^3 + 27*p^2 - 30*p + 9)*(p-5)!
	//(p-5)!=(p-4)!(p-4)^-1=(p-2)^-1*(p-3)^-1*(p-4)^1=(p-24)^-1
	return mmi(p - 24, p)*9%p;
}

int main(void){
	size_t ps_len;
	uint64_t *ps = primesieve_generate_primes(24, MAX, &ps_len, UINT64_PRIMES);
	//S only works on numbers >= 24 so we need to pre calculate 5, 7, 11, 13, 17, 19, 23
	uint64_t s = 42;
#pragma omp parallel for reduction(+:s)
	for(size_t i = 0; i < ps_len; ++i){
		s += S_mmi(ps[i]);
	}
	primesieve_free(ps);
	printf("%"PRIu64"\n", s);
}
