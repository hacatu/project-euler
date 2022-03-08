#include <stdio.h>
#include <inttypes.h>
#include <string.h>

static inline void mul_mod_pcp(int64_t c[restrict static 3], const int64_t a[restrict static 3], const int64_t b[restrict static 3], int64_t n, int64_t m){
	c[0] = a[0]*b[0];
	c[1] = a[0]*b[1] + a[1]*b[0];
	c[2] = a[0]*b[2] + a[1]*b[1] + a[2]*b[0];
	int64_t A = a[2]*b[2]%m;
	c[1] -= n*A;
	A <<= n;
	A += a[1]*b[2] + a[2]*b[1];
	A %= m;
	c[0] -= n*A;
	c[2] += A << n;
	c[0] %= m;
	c[1] %= m;
	c[2] %= m;
}

static inline void square_mod_pcp(int64_t c[restrict static 3], const int64_t a[restrict static 3], int64_t n, int64_t m){
	c[0] = a[0]*a[0];
	c[1] = 2*a[0]*a[1];
	c[2] = 2*a[0]*a[2] + a[1]*a[1];
	int64_t A = a[2]*a[2]%m;
	c[1] -= n*A;
	A <<= n;
	A += 2*a[1]*a[2];
	A %= m;
	c[0] -= n*A;
	c[2] += A << n;
	c[0] %= m;
	c[1] %= m;
	c[2] %= m;
}

static inline int64_t ak(int64_t k, int64_t n, int64_t m){
	int64_t _poly_s[3] = {[1]=1};
	int64_t _poly_r[3], _poly_t[3];
	int64_t *poly_s = _poly_s, *poly_r = _poly_r, *poly_t = _poly_t;
	int64_t *tmp;
	int64_t r = k - 2;
	// [p_k    ]   [2^n 0 -n]^{k-2}   [2^{2*n}]
	// [p_{k-1}] = [1   0  0]       * [2^n    ]
	// [p_{k-2}]   [0   1  0]         [1      ]
	// so we compute the matrix power by working in Z_m/(x^3-2^n*x^2+n)
	while(r%2 == 0){
		square_mod_pcp(poly_t, poly_s, n, m);
		tmp = poly_s;
		poly_s = poly_t;
		poly_t = tmp;
		r >>= 1;
	}
	memcpy(poly_r, poly_s, 3*sizeof(int64_t));
	while((r >>= 1)){
		square_mod_pcp(poly_t, poly_s, n, m);
		if(r%2){
			mul_mod_pcp(poly_s, poly_t, poly_r, n, m);
			tmp = poly_s;
			poly_s = poly_t;
			poly_t = poly_r;
			poly_r = tmp;
		}else{
			tmp = poly_s;
			poly_s = poly_t;
			poly_t = tmp;
		}
	}
	/*
	for(uint64_t i = 0; i < 3; ++i){
		printf("%"PRId64" ", poly_r[i]);
	}
	printf("\n");
	*/
	// plug in the matrix to poly_r, which now holds ax^2 + bx + c = x^{k-2} mod x^3-2^n*x^2+n
	// then pk is a*2^n*(2^{3*n}-4*n) + b*(2*{3*n}-3*n) + c*2**n
	int64_t ttn = (1ll << (2*n))%m;
	int64_t x = ((ttn << n) - 4*n)%m;
	x = (x << n)%m;// x = 2^n*(2^{3*n}-4*n)
	int64_t res = poly_r[2]*x%m;
	x = ((ttn << n) - 3*n)%m;// x = 2^{3*n} - 3*n
	res = (res + poly_r[1]*x + poly_r[0]*ttn - 1)%m;// c is multiplied by 2^{2*n}
	return res >= 0 ? res : res + m;
}

int main(){
	int64_t s = 0;
	for(uint64_t n = 1; n <= 30; ++n){
		s += ak(987654321, n, 100000000);
	}
	s %= 100000000;
	printf("%"PRId64"\n", s);
}

