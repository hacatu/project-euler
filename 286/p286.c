#include <stdio.h>
#include <flint/fmpz_poly.h>
#include <arb_fmpz_poly.h>

int main(){
	fmpz_poly_struct _pdf1[21] = {};
	fmpz_poly_struct _pdf2[21] = {};
	for(slong i = 0; i < 21; ++i){
		fmpz_poly_init(_pdf1 + i);
		fmpz_poly_init(_pdf2 + i);
	}
	fmpz_poly_struct *pdf1 = _pdf1;
	fmpz_poly_struct *pdf2 = _pdf2;
	void *tmp;
	fmpz_poly_one(pdf1 + 0);
	fmpz_poly_t sp, fp, tp;
	fmpz_poly_init(sp);
	fmpz_poly_one(sp);
	fmpz_poly_init(fp);
	fmpz_poly_zero(fp);
	fmpz_poly_init(tp);
	for(slong i = 1; i < 51; ++i){
		fmpz_poly_set_coeff_si(fp, 1, i);
		fmpz_poly_set_coeff_si(sp, 1, -i);
		fmpz_poly_mul(pdf2 + 0, pdf1 + 0, fp);
		slong ub = 21;
		if(i < 21){
			fmpz_poly_mul(pdf2 + i, pdf1 + i - 1, sp);
			ub = i;
		}
		for(slong j = 1; j < ub; ++j){
			fmpz_poly_mul(pdf2 + j, pdf1 + j - 1, sp);
			fmpz_poly_mul(tp, pdf1 + j, fp);
			fmpz_poly_add(pdf2 + j, pdf2 + j, tp);
		}
		tmp = pdf1;
		pdf1 = pdf2;
		pdf2 = tmp;
	}
	fmpz_poly_scalar_mul_si(pdf1 + 20, pdf1 + 20, 50);
	fmpz_poly_set_coeff_si(pdf1 + 20, 0, -1);
	//fmpz_poly_print_pretty(pdf1 + 20, "a");
	//printf("\n");
	acb_ptr roots = _acb_vec_init(50);
	arb_fmpz_poly_complex_roots(roots, pdf1 + 20, 0, 50);
	arb_t q;
	arb_init(q);
	arb_inv(q, acb_realref((roots + 1)), 52);
	arb_printd(q, 14);
	printf("\n");
}

