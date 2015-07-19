#include <stdio.h>

/* b*(b-1) 1
 * -------=- => 2*b^2-2*b=n^2-n => 0=2*b^2-2*b-n^2+n (first solution: n=4, b=3)
 * n*(n-1) 2
 * 
 *   2(+/-)sqrt(4-8(-n*2+n))      1+sqrt(2*n^2-2*n+1)
 * b=----------------------- => b=-------------------
 *             4                          2
 * 
 * 2*n^2-2*n+1 is square
 * 2*(n^2-n+1/2) is square
 * 2*((n-1/2)^2+1/4) is square
 * 2*(2*n-1)^2+2 is square (multiplied by 2^2)
 * v = 2*n-1
 * u^2 = 2*v^2+2
 * u^2-2*v^2=2
 * 
 * if u^2-d*v^2=(+/-)2 then (u^2(-/+)1)^2-d*(u*v)^2=1
 * 
 * Pell's equation: x^2-d*y^2=1
 * x=u^2-1
 * y=u*v
 * d=2
 * 
 * n_1=4
 * v_1=7
 * u_1^2=100
 * u_1=10
 * x_1=99
 * y_1=70
 * 
 * x_n=x_1*x_(n-1)+d*y_1*y_(n-1)
 * y_n=x_1*y_(n-1)+  y_1*x_(n-1)
 */

int main(void){
	
}

