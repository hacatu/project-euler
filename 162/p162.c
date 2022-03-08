#include <stdio.h>
#include <gmp.h>

/* Number must include 0, 1, and A
 * N = 20
 * Count all numbers of the form (right to left)
 * 01A: [^01A]{G} 0 [^1A]{H} 1 [^A]{I} A {J}
 * 0A1: [^01A]{G} 0 [^1A]{H} A [^1]{I} 1 {J}
 * 10A: [^01A]{G} 1 [^0A]{H} 0 [^A]{I} A {J}
 * A01: [^01A]{G} A [^01]{H} 0 [^1]{I} 1 {J}
 * 1A0: [^01A]{G} 1 [^0A]{H} A [^0]{I} 0 {J}<not all 0>
 * A10: [^01A]{G} A [^01]{H} 1 [^0]{I} 0 {J}<not all 0>
 * 
 * 4 * 13^G * 14^H * 15^I * 16^J where G + H + I + J + 3 = N
 * 2 * 13^G * 14^H * 15^I * (16^J - 1) where G + H + I + J + 3 = N
 * 
 * sum(sum(sum(4*13^G*14^H*15^I*16^(N - 3 - G - H - I) + 2*13^G*14^H*15^I*(16^(N - 3 - G - H - I) - 1), I, 0, N - 3 - G - H), H, 0, N - 3 - G), G, 0, N - 3)
 * (1092*16^n - 3354*15^n + 3444*14^n - 1183*13^n + 1)/1092
 */

void ways(mpz_t out, unsigned long n){
	mpz_t t;
	mpz_init(t);
	mpz_set_ui(out, 1);
	mpz_ui_pow_ui(t, 16, n);
	mpz_addmul_ui(out, t, 1092);
	mpz_ui_pow_ui(t, 15, n);
	mpz_submul_ui(out, t, 3354);
	mpz_ui_pow_ui(t, 14, n);
	mpz_addmul_ui(out, t, 3444);
	mpz_ui_pow_ui(t, 13, n);
	mpz_submul_ui(out, t, 1183);
	mpz_fdiv_q_ui(out, out, 1092);
}

int main(void){
	mpz_t c;
	mpz_init(c);
	ways(c, 16);
	printf("%lX\n", mpz_get_ui(c));
}

