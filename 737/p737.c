/* a_n = acos(r_(n-1)/2)-p_(n-1)
 * c_n = r_n*e^(i*p_n) = x_n + i*y_n = ((n-1)*c_(n-1)*e^(i*a_n) + 1)/n
 *     = ((n-1)*r_(n-1)*e^(i*(p_(n-1)+a_n)) + 1)/n
 *     = ((n-1)*r_(n-1)*e^(i*acos(r_(n-1)/2)) + 1)/n
 *     = ((n-1)*r_(n-1)^2 + 2)/(2*n) + (n-1)*r_(n-1)*sqrt(4 - r_(n-1)^2)*i/(2*n)
 * r_n^2 = ((n-1)^2*r_(n-1)^4 + 4*(n-1)*r_(n-1)^2 + 4 + 4*(n-1)^2*r_(n-1)^2 - (n-1)^2*r_(n-1)^4)/(4*n^2)
 *       = (n*(n-1)*r_(n-1)^2 + 1)/n^2
 *       = 1/n*sum_(k=1)^n (1/k)
 */

#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

const long double m_pi_3 = 1.0471975511965977461542144610931676280657231331250352736583148641L;
//const long double rot = 6346.017160251382L;
const long double rot = 6.28318530717L;

void stack_coins(uint64_t N){
	long double r2 = 1, p = 0, r = 1, x, y, t = m_pi_3, s = 0;
	for(uint64_t n = 2; s <= rot; ++n){
		s += t;
		printf("  %Lf(%Lf)\n", s, t);
		t = acosl((2*n - 1)*r/(2*sqrtl(n*(n-1)*r2 + 1)));
		r2 = (n*(n-1)*r2 + 1)/(n*n);
		r = sqrtl(r2);
		p = acosl((n*n*r2 + 2*n - 1)/(2*n*n*r));
		x = r*cosl(p);
		y = r*sinl(p);
		printf("%"PRIu64"(%Lf,%Lf)=%Lf*exp(i*%Lf)\n", n, x, y, r, p);
	}
}

int main(){
	stack_coins(69470000);
}

//174451028