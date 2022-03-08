#define _GNU_SOURCE

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>

#include <acb_modular.h>
#include <flint/flint.h>
#include <flint/fmpz_poly.h>

complex double eta24_nome(complex double q){
	//for a double, at most 13 terms of the sum for the descriminant
	//are needed.
	//eta24(q) = q*(1 + sum_{n>=1}((-1)^n)*(q^(n*(3n-1)/2) + q^(n*(2n+1)/2)))^24
	complex double s = 0;
	for(int64_t n = 24; n >= 0; --n){
		complex double t = (2*n + 1)*cpow(q, (2*n + 1)*(2*n + 1)*0.125);
		if(n&1){
			s -= t;
		}else{
			s += t;
		}
	}
	return cpow(s, 8);
}

/*
complex double eta24_nome(complex double q){
	complex double s = 0;
	for(int64_t n = 24; n <= 1; --n){
		complex double t = cpow(q, (3*n-1)*0.5) + cpow(q, (3*n+1)*0.5);
		if(n&1){
			s -= t;
		}else{
			s += t;
		}
	}
	s += 1;
	return q*cpow(s, 24);
}
*/

complex double f24_2_nome(complex double q){
	return 4096*eta24_nome(q*q)/eta24_nome(q);
}

complex double j_nome(complex double q){
	return cpow(f24_2_nome(q) + 16, 3)/f24_2_nome(q);
}

complex double j_z(complex double z){
	return j_nome(cexp(M_PI*2*I*z));
}

complex double j_C(int64_t a, int64_t b, int64_t D){
	complex double z = csqrt(D);
	z -= b;
	z /= 2*a;
	return j_z(z);
}

int main(){
	/*
	int64_t a, b, D = -23;
	complex double j;
	printf("D = %"PRId64"\n", D);
	*/
	/*
	a = 1, b = 1;
	j = j_C(a, b, D);
	printf("j(%"PRId64", %"PRId64") = %f + %fi\n", a, b, creal(j), cimag(j));
	a = 2, b = -1;
	j = j_C(a, b, D);
	printf("j(%"PRId64", %"PRId64") = %f + %fi\n", a, b, creal(j), cimag(j));
	a = 2, b = 1;
	j = j_C(a, b, D);
	printf("j(%"PRId64", %"PRId64") = %f + %fi\n", a, b, creal(j), cimag(j));
	*/
	/*
	D = -7;
	a = 1, b = 1;
	j = j_C(a, b, D);
	printf("j(%"PRId64", %"PRId64") = %f + %fi\n", a, b, creal(j), cimag(j));
	*/
	fmpz_poly_t H_D;
	fmpz_poly_init(H_D);
	for(slong D = -4; D >= -10000; D -= 4){
		acb_modular_hilbert_class_poly(H_D, D + 1);
		flint_printf("H_{%wd}(X)=", D + 1);
		fmpz_poly_print_pretty(H_D, "x");
		printf("\n");
		flint_printf("H_{%wd}(X)=", D);
		acb_modular_hilbert_class_poly(H_D, D);
		fmpz_poly_print_pretty(H_D, "x");
		printf("\n");
	}
	fmpz_poly_clear(H_D);
	flint_cleanup();
}

