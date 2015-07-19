#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

int main (){
  mpz_t z, p;
  mpz_init(z);
  mpz_init(p);
  mpz_ui_pow_ui(p, 10, 198);
  unsigned long s = 0;
  for(unsigned long n = 2; n < 100; ++n){
	  if(n == 4 || n == 9 || n == 16 || n == 25 || n == 36 || n == 49 || n == 64 || n == 81){
		  continue;
	  }
	  mpz_mul_ui(z, p, n);
	  mpz_sqrt(z, z);
	  /*
	  printf("%02lu ", n);
	  mpz_out_str(stdout, 10, z);
	  puts("");
	  */
	  for(unsigned long i = 0; i < 100; ++i){
		  s += mpz_fdiv_q_ui(z, z, 10);
	  }
  }
  printf("%lu\n", s);
}
/*
1414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641572
*/

