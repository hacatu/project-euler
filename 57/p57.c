#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

int main(){
	mpq_t expansion, one;
	mpq_init(expansion);
	mpq_init(one);
	mpq_set_ui(expansion, 1, 1);
	mpq_set_ui(one, 1, 1);
	mpz_t z;
	mpz_init(z);
	unsigned long t;
	int c = 0;
	for(int i = 0; i < 800; ++i){
		mpq_add(expansion, one, expansion);
		mpq_inv(expansion, expansion);
		mpq_add(expansion, one, expansion);
		mpq_canonicalize(expansion);
		mpq_get_num(z, expansion);
		mpz_out_str(stdout, 10, z);
		printf("/");
		t = (unsigned long)log10(mpz_get_d(z));
		mpq_get_den(z, expansion);
		mpz_out_str(stdout, 10, z);
		puts("");
		if(t > (unsigned long)log10(mpz_get_d(z))){
			++c;
		}
	}
	printf("%i\n", c);
	mpz_clear(z);
	mpq_clear(one);
	mpq_clear(expansion);
}
