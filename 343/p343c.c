#define _GNU_SOURCE

#include <stdio.h>

#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fmpz_factor.h>

int main(){
	fmpz_factor_t factors;
	fmpz_factor_init(factors);
	slong s = 1;
	for(slong k = 2; k <= 2000000; ++k){
		fmpz_factor_si(factors, k + 1);
		slong m = 0;
		for(slong i = 0; i < factors->num; ++i){
			m = FLINT_MAX(m, fmpz_get_si(factors->p + i));
		}
		fmpz_factor_si(factors, k*k - k + 1);
		for(slong i = 0; i < factors->num; ++i){
			m = FLINT_MAX(m, fmpz_get_si(factors->p + i));
		}
		s += m - 1;
	}
	fmpz_factor_clear(factors);
	flint_printf("%wd\n", s);
	flint_cleanup();
}

