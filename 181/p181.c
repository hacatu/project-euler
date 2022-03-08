/*
Number of groupings of b black and w white objects can be thought of using
generating functions and symmetric polynomials.  I'm not familiar enough with
symmetric polynomials, but by hacking around with the generating function for
unordered ways of making change, we see that

[x^b]\frac{1}{\prod_{i=1}^\inf(1-x^i)}=[x^b]\prod_{i=0}^\inf\sum_{k=0}^\inf x^{ik}

So we can extend to multiple object types as

[x^b y^w]\prod_{(i,j)\ge(0,0)}^{i\le b\land j\le w}\sum_{k=0}^{\min(\lfloor\frac{b}{i}\rfloor,\lfloor\frac{w}{j}\rfloor)} (x^i y^j)^k

I put in limits on the powers as well, we can notice that we only need to track coefficients on monomials x^i y^j with i \le b and j \le w
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

void bounded_poly_geo_product(uint64_t b, uint64_t w, const uint64_t coeffs[static (b + 1)*(w + 1)], uint64_t output[static (b + 1)*(w + 1)], uint64_t i0, uint64_t j0){
	memset(output, 0, (b + 1)*(w + 1)*sizeof(uint64_t));
	for(uint64_t k = 0; k*i0 <= b && k*j0 <= w; ++k){
		for(uint64_t i = 0; k*i0 + i <= b; ++i){
			for(uint64_t j = 0; k*j0 + j <= w; ++j){
				output[(w + 1)*(k*i0 + i) + k*j0 + j] += coeffs[(w + 1)*i + j];
			}
		}
	}
}

void print_coeffs(uint64_t b, uint64_t w, const uint64_t coeffs[static (b + 1)*(w + 1)]){
	for(uint64_t i = 0; i <= b; ++i){
		for(uint64_t j = 0; j <= w; ++j){
			if(coeffs[(w + 1)*i + j]){
				printf("%"PRIu64"x^%"PRIu64"y^%"PRIu64" + ", coeffs[(w + 1)*i + j], i, j);
			}
		}
	}
	puts("0");
}

uint64_t product_coeff(uint64_t b, uint64_t w){
	uint64_t *coeffs_a = calloc((b + 1)*(w + 1), sizeof(uint64_t));
	uint64_t *coeffs_b = malloc((b + 1)*(w + 1)*sizeof(uint64_t));
	uint64_t *coeffs_t;
	for(uint64_t k = 0; k <= w; ++k){
		coeffs_a[k] = 1;
	}
	for(uint64_t j0 = 2; j0 <= w; ++j0){
		//puts("multiplying");
		//print_coeffs(b, w, coeffs_a);
		//printf("by (1 + y^%"PRIu64" + ...)\n", j0);
		bounded_poly_geo_product(b, w, coeffs_a, coeffs_b, 0, j0);
		coeffs_t = coeffs_b;
		coeffs_b = coeffs_a;
		coeffs_a = coeffs_t;
	}
	for(uint64_t i0 = 1; i0 <= b; ++i0){
		for(uint64_t j0 = 0; j0 <= w; ++j0){
			//puts("multiplying");
			//print_coeffs(b, w, coeffs_a);
			//printf("by (1 + x^%"PRIu64"y^%"PRIu64" + ...)\n", i0, j0);
			bounded_poly_geo_product(b, w, coeffs_a, coeffs_b, i0, j0);
			coeffs_t = coeffs_b;
			coeffs_b = coeffs_a;
			coeffs_a = coeffs_t;
		}
	}
	free(coeffs_b);
	uint64_t res = coeffs_a[(w + 1)*b + w];
	free(coeffs_a);
	return res;
}

int main(){
	printf("%"PRIu64"\n", product_coeff(60, 40));
}

