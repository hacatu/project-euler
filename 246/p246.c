/*
We start with an ellipse:
(x-x0)^2/a^2 + (y-y0)^2/b^2 = 1

Then for a given point P = (k, h), every line through P
(except vertical lines) has the equation y = m*(x-k) + h for some m.
Thus the slopes of the tangents to the ellipse through P are values
of m such that the line intersects the ellipse at exactly one point,
ie the following equation has one x solution:
(x-x0)^2/a^2 + (m*(x-k) + h - y0)^2/b^2 = 1
b^2*(x-x0)^2 + a^2*(m*(x-k) - (y0-h))^2 = a^2*b^2
b^2*x^2 - 2*b^2*x0*x + b^2*x0^2 + a^2*m^2*(x-k)^2 - 2*a^2*(y0-h)*m*(x-k) + a^2*(y0-h)^2 = a^2*b^2
***     a^2*m^2*(x-k)^2 - 2*a^2*(y0-h)*m*(x-k)
***     a^2*m^2*x^2 - 2*a^2*k*m^2*x + a^2*k^2*m^2 - 2*a^2*(y0-h)*m*x + 2*a^2*(y0-h)*k*m
0 = b^2*x^2 - 2*b^2*x0*x + b^2*x0^2 + a^2*m^2*x^2 - 2*a^2*k*m^2*x + a^2*k^2*m^2 - 2*a^2*(y0-h)*m*x + 2*a^2*(y0-h)*k*m + a^2*(y0-h)^2 - a^2*b^2

Now we can check the discriminant of the quadratic formula with A, B, and C taken from the above (B^2 = 4*A*C):
(-2*b^2*x0 - 2*a^2*k*m^2 - 2*a^2*(y0-h)*m)^2 = 4*(b^2 + a^2*m^2)*(b^2*x0^2 + a^2*k^2*m^2 + 2*a^2*(y0-h)*k*m + a^2*(y0-h)^2 - a^2*b^2)
(a^2*k*m^2 + a^2*(y0-h)*m + b^2*x0)^2 = (b^2 + a^2*m^2)*(a^2*k^2*m^2 + 2*a^2*(y0-h)*k*m + a^2*(y0-h)^2 + b^2*x0^2 - a^2*b^2)
m term breakdown:
m^4: a^4*k^2*m^4 = a^4*k^2*m^4 (cancels)
m^3: 2*a^4*(y0-h)*k*m^3 = 2*a^4*(y0-h)*k*m^3 (cancels)
m^2: 2*a^2*b^2*k*x0*m^2 + a^4*(y0-h)^2*m^2 = a^2*(a^2*(y0-h)^2 + b^2*x0^2 + b^2*k^2 - a^2*b^2)*m^2
     0 = a^2*b^2*((x0-k)^2 - a^2)*m^2
m^1: 2*a^2*b^2*(y0-h)*x0*m = 2*a^2*b^2*(y0-h)*k*m
m^0: b^4*x0^2 = b^2*(a^2*(y0-h)^2 + b^2*x0^2 - a^2*b^2)
     0 = a^2*b^2*((y0-h)^2 - b^2)
full equation:
2*a^2*b^2*(y0-h)*x0*m = 2*a^2*b^2*(y0-h)*k*m + a^2*b^2*((x0-k)^2 - a^2)*m^2 + a^2*b^2*((y0-h)^2 - b^2)
0 = a^2*b^2*((x0-k)^2 - a^2)*m^2 - 2*a^2*b^2*(y0-h)*(x0-k)*m + a^2*b^2*((y0-h)^2 - b^2)
0 = ((x0-k)^2 - a^2)*m^2 - 2*(y0-h)*(x0-k)*m + (y0-h)^2 - b^2

m = ((y0-h)*(x0-k) +- sqrt((y0-h)^2*(x0-k)^2 - ((x0-k)^2 - a^2)*((y0-h)^2 - b^2)))/((x0-k)^2 - a^2)
m = ((y0-h)*(x0-k) +- sqrt((y0-h)^2*(x0-k)^2 - (y0-h)^2*(x0-k)^2 + a^2*(y0-h)^2 + b^2*(x0-k)^2 - a^2*b^2)/((x0-k)^2 - a^2)
m = ((y0-h)*(x0-k) +- sqrt(a^2*(y0-h)^2 + b^2*(x0-k)^2 - a^2*b^2)/((x0-k)^2 - a^2)

Next, we use the tangent difference formula to express the angle between the tangents:
tan(th) = (m1 - m2)/(1 + m1*m2)

In particular, the difference between the slopes is twice the +-'d part in the quadratic formula,
and the product of the slopes is C/A for the m equation:
th = arctan((2*sqrt(a^2*(y0-h)^2 + b^2*(x0-k)^2 - a^2*b^2)/((x0-k)^2 - a^2))/(1 + (())/()))

https://www.desmos.com/calculator/xsdgelyzqj
*/

#include <stdio.h>
#include <inttypes.h>

uint64_t T_outer1(){
	uint64_t T = 0;
	for(uint64_t k = 1, hc = 18950; k <= 12248; ++k){
		T += hc - 1;
		//compute the next hc, decrement and test is probably best
	}
}

int main(int argc, char **argv){
	
}

