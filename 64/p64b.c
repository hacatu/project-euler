#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

uint64_t coef(uint64_t r, uint64_t n, uint64_t d){
	return n/(sqrt(r)-d);
}

/* iterate returns the next a_i value, starting at a_0.
 * it also updates n and d to the next partial fraction
 * //values
 * c: a_i //calculated by coef()
 * r: radical
 * n: numerator
 * d: denominator
 * //partial fractions are like n/(sqrt(r) + d)
 * //algorithm
 * 1: n/(sqrt(r) + d)
 *     //subtract a_i (c)
 * 2: n/(sqrt(r) + d) - c
 *     //take the reciporacal
 * 3: 1/(n/(sqrt(r) + d) - c)
 *     //rationalize the inner fraction (multiply by the conjugate) and combine:
 * 4: 1/((n*sqrt(r) - n*d - c*r +c*d^2)/(r - d^2))
 *     //replace 1/(a/b) with b/a
 * 5: (r - d^2)/(n*sqrt(r) - n*d - c*r + c*d^2)
 *     //divide top and bottom by n (this will always go in evenly)
 * 6: ((r - d^2)/n)/(sqrt(r) - d - c*(r - d^2)/n)
 * 
 * This is basically the Euclidean GCD algorithm except it finds the GDC between sqrt(r) and 1 and so never terminates.
 * n = r and d = 0 at the beginning.
 * 
 * This means that:
 * n -->    (r - d^2)/n
 * d --> -c*(r - d^2)/n - d
 * 
 * So we can use:
 * n = (r - d*d)/n;
 * d = -c*n - d;
 */
uint64_t iterate(uint64_t r, uint64_t *n, uint64_t *d){
	uint64_t c = coef(r, *n, *d);
	*n = (r - (*d)*(*d))/(*n);
	*d = c*(*n) - (*d);
	return c;
}

int main(){
	uint64_t odd = 0;
	for(uint64_t i = 2, r, c, n, d, n0, d0; i < 10000; ++i){
		r = floor(sqrt(i));
		if(i == r*r){
			continue;
		}
		c = 0;
		n = i;
		d = 0;
		iterate(i, &n, &d);
		n0 = n;
		d0 = d;
		do{
			++c;
			iterate(i, &n, &d);
		}while(n != n0 || d != d0);
		if(c & 1){
			++odd;
		}
	}
	printf("%" PRId64 "\n", odd);
}

