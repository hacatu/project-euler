#include <stdio.h>
#include <flint/fmpz_poly.h>
#include <flint/fmpz_vec.h>

void count_polys_naive(slong k, slong r, fmpz counts[static k + 1]){
	slong t = k;
	_fmpz_vec_zero(counts, t + 1);
	fmpz *powers = _fmpz_vec_init(k + 1);
	fmpz_set_si(powers + 0, 1);
	for(slong i = 1; i < k + 1; ++i){
		fmpz_mul_si(powers + i, powers + i - 1, r);
	}
	fmpz *coeffs = _fmpz_vec_init(k + 1);
	fmpz_t i, px, t1, tmp;
	fmpz_init_set_si(i, 0);
	fmpz_init(px);
	fmpz_init_set_si(t1, t + 1);
	fmpz_init(tmp);
	while(1){
		if(fmpz_is_zero(px)){
			_fmpz_vec_height(tmp, coeffs, k + 1);
			slong j = fmpz_get_si(tmp);
			fmpz_add_si(counts + j, counts + j, 1);
		}
		fmpz_add_si(i, i, 1);
		slong j = fmpz_remove(tmp, i, t1);
		if(j == k + 1){
			break;
		}
		slong c = fmpz_get_si(coeffs + j);
		if(++c == t + 1){
			c = 0;
			fmpz_addmul_si(px, powers + j, -t);
		}else{
			fmpz_add(px, px, powers + j);
		}
		fmpz_set_si(coeffs + j, c);
	}
	fmpz_clear(i);
	fmpz_clear(px);
	fmpz_clear(t1);
	fmpz_clear(tmp);
	_fmpz_vec_clear(powers, k + 1);
	_fmpz_vec_clear(coeffs, k + 1);
	for(slong j = t - 1; j >= 0; --j){
		for(slong o = j + 1; o < t + 1; ++o){
			fmpz_add(counts + o, counts + o, counts + j);
		}
	}
}

int main(){
	slong max_k = 9;
	fmpz *counts = _fmpz_vec_init(max_k + 1);
	for(slong k = 1; k < max_k + 1; ++k){
		flint_printf("%wd: [", k);
		count_polys_naive(k, -1, counts);
		for(slong t = 0; t < k + 1; ++t){
			fmpz_print(counts + t);
			if(t < k){
				printf(", ");
			}
		}
		printf("]\n");
	}
	_fmpz_vec_clear(counts, max_k + 1);
}

