#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/*
 * x_1^1 * x_2^2 * ... * x_m^m = max such that x_1 + x_2 + ... + x_m = m
 * 1*ln(x_1) + 2*ln(x_2) + ... + m*ln(x_m) = max such that x_1 + x_2 + ... x_m = m
 * integral(x*ln(y), x, 1, m) = max such that integral(y, x, 1, m) = m
 * I(y) = integral(F(y, y', x))
 * F(y, y', x) = x*ln(y), G(y, y', x) = y, H(y, y', x) = x*ln(y) - l*y
 * (d/dx)*(pH/py') - (pH/py) = 0
 * x/y - l = 0
 * y = x/l
 * integral(x/l, x, 1, m) = m
 * m^2/(2*l) - 1/(2*l) = m
 * l = (m^2 - 1)/(2*m)
 * y = 2*m*x/(m^2 - 1)
 * x_i = 2*m*i/(m^2 - 1)
 * I(y) = integral(2*m*x/(m^2 - 1), x, 1, m) = (m^3 - m)/(m^2 - 1) = m
 * e^I(y) = e^m
 */

int gje(size_t n, mpq_t A[n][n + 1]){
	
}

int main(void){
	
}

