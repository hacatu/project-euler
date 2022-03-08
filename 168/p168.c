#include <stdio.h>
#include <gmp.h>

/*
 * n = 10*a + b
 * d*n = 10^l*b + a
 * d*(10*a + b) = 10^l*b + a
 * d and b are single digits, so solve for a
 * 10*d*a + b*d = 10^l*b + a
 * (10*d - 1)*a = (10^l - d)*b
 * a = (10^l - d)*b/(10*d - 1)
 * a must have l digits, so
 * 10^l > a >= 10^(l-1)
 */

int main(void){
	mpz_t t, a;
	mpz_inits(t, a, NULL);
	int s = 0, c = 0;
	for(int d = 1, l; d <= 9; ++d){
		for(l = 1, mpz_set_ui(t, 10); l < 100; ++l, mpz_mul_ui(t, t, 10)){
			for(int b = 1; b <= 9; ++b){
				mpz_sub_ui(a, t, d);
				mpz_mul_ui(a, a, b);
				if(mpz_divisible_ui_p(a, 10*d - 1)){
					mpz_divexact_ui(a, a, 10*d - 1);
					if(mpz_cmp(t, a) <= 0){
						break;
					}
					mpz_mul_ui(a, a, 10);
					if(mpz_cmp(a, t) < 0){
						continue;
					}
					int n = mpz_fdiv_ui(a, 100000) + b;
					s = (s + n)%100000;
					++c;
				}
			}
		}
	}
	printf("%i\n", s);
}

