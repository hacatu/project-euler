#include <stdio.h>
#include <math.h>
#include <arb.h>
#include <arf.h>

/*
static const ulong _is_square_256[4];

static void __attribute__((constructor)) _init_tables(){
	for(ulong q = 0; q < 128; ++q){
		_is_square_256[q*q%256/64] |= 1ull << (q*q%64);
	}
}

static inline int is_square(ulong q){
	if(!(_is_square_256[q*q%256/64] & (1ull << (q*q%64)))){
		return 0;
	}
	return __builtin_popcount(q%7) <= 1;
}
*/

static const slong prec = 52;

static inline void N(arb_t z, ulong p, ulong q, const arb_t d){
	arb_sqrt_ui(z, 4*p*q, prec);
	arb_sub_ui(z, z, p+q, prec);
	arb_neg(z, z);
	arb_log(z, z, prec);
	arb_div(z, d, z, prec);
}

int main(){
	arb_t z, d;
	arb_init(z);
	arb_init(d);
	arb_log_ui(d, 10, prec);
	arb_mul_si(d, d, -2011, prec);
	slong s = 0;
	for(ulong q = 2; q <= 1037; ++q){
		for(ulong p = q - 1 <= 2011 - q ? q - 1 : 2011 - q; p > (ulong)floor(q + 1 - 2*sqrt(q)); --p){
			N(z, p, q, d);
			if(arb_contains_int(z)){
				printf("\e[1;31mN(%lu, %lu) was not computed to sufficient precision\e[0m\n", p, q);
			}else{
				slong n = arf_get_si(arb_midref(z), ARF_RND_UP);
				//printf("\e[1;32mN(%lu, %lu) = %ld\e[0m\n", p, q, n);
				s += n;
			}
		}
	}
	printf("%ld\n", s);
	arb_clear(z);
	arb_clear(d);
}

