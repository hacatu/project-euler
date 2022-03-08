#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/*
 * c**2 = a**2 + b**2 - 2ab*cos(C) //Law of cosines
 * c**2 = a**2 + b**2 - ab //cos(60) = 1/2
 * This equation is an ellipse with diagonal axes.
 * To eliminate the cross term, substitute a, b = x + y, -x + y.
 * This is equivalent to a 45 degree rotation and scaling by sqrt(2).
 * This results in the equation
 * c**2 = x**2 + 3y**2
 * This is a rational Pell's equation x**2 + 3y**2 = 1, and the solutions are
 * x, y = (u**2 - 3v**2)/(u**2 + 3v**2), 2uv/(u**2 + 3v**2)
 * So c = u**2 + 3v**2, a = u**2 + 2uv - 3v**2, and b = -u**2 + 2uv + 3v**2
 * 
 * --EDIT: the substitution actually yields c**2 = 3*x**2 + y**2 ?
 * --I'm not sure if that's how I messed it up but since my formula is
 * --producing 120 degree triangles I know I have some problem.
 * --I assume I followed https://math.stackexchange.com/a/2039292/72599
 * --I will do that now explicitly.  If we pick an initial rational solution
 * --to 3*x**2 + y**2 = 1, such as the solution corresponding to the triangle a = b = c = 1,
 * --namely x = 1, y = 0, and take any rational direction (n, m) with n, m coprime
 * --(n and m will be changed to u and v once this derivation is complete since of course n is the maximum radius)
 * --and look for solutions (x, y) = (x_0, y_0) + t*(n, m)
 * --to 3*(x_0 + t*n)**2 + (y_0 + t*m)**2 = 1
 * --substitute x_0 and y_0
 * --3*(1 + t*n)**2 + (t*m)**2 = 1
 * --(3 + 6*n*t + 3*n**2*t**2) + (m**2*t**2) = 1
 * --(3*n**2 + m**2)*t**2 + 6*n*t + 2 = 0
 * --but obviously this requires square roots to solve for t so it won't get
 * --us rational points.  We can arbitrarily change the 1 to a 3 and see what happens,
 * --it should produce solutions but I expect they would all be rational multiples of sqrt(3) or something
 * --(3*n**2 + m**2)*t**2 + 6*n*t = 0
 * --t*((3*n**2 + m**2)*t + 6*n) = 0
 * --(3*n**2 + m**2)*t + 6*n = 0
 * --t = -6*n/(3*n**2 + m**2)
 * --x = x_0 + n*t = (3*n**2 + m**2 - 6*n**2)/(3*n**2 + m**2)
 * --y = y_0 + m*t = -6*n*m/(3*n**2 + m**2)
 * --
 * --and already this is looking pretty wack but let's bring it back to a, b, and c
 * --a = (3*n**2 + m**2 - 6*n**2 - 6*n*m)/(3*n**2 + m**2)
 * --b = (-3*n**2 - m**2 + 6*n**2 - 6*n*m)/(3*n**2 + m**2)
 * --c = sqrt(3)
 * --and here we already have a bit of a pickle because we started with an initial point corresponding to a 1, 1, 1 triangle
 * --which this clearly isn't, and c is irrational.  Continuing to ignore the obvious problems,
 * --multiply through by the denominator and simplify
 * --a = -3*u**2 + v**2 - 6*u*v
 * --b = 3*u**2 - v**2 - 6*u*v
 * --c = (3*u**2 + v**2)*sqrt(3)
 * --and if we plug in, um, ugh
 * --we want to plug in some u and v so that a and b are positive and go from there
 * --a: v**2 - 6*u*v - 3*u**2 >= 1
 * --   v = (6*u +/- sqrt(36*u**2 + 12*u**2))/2 = (3 - 2*sqrt(3))*u
 * --b: 3*u**2 - 6*u*v - v**2 >= 1
 * --   v = (-6*u +/- sqrt(36*u**2 + 12*u**2))/2 = (-3 - 2*sqrt(3))*u
 * --I chose the negative branches because quadrant 4 in the uv plane contains valid uv pairs
 * --I will negate v if this works to make it make sense.
 * --So we can choose any uv pair in this region, and the smallest one is (1, -1)
 * --which leads to a = -3 + 1 + 6 = 4 and b = 3 - 1 + 6 = 8
 * --and c = 4*sqrt(3)
 * --this does not even form a triangle
 * --if we make c 3* whatever instead of sqrt(3)* whatever, this case would be a triangle but not a 60 degree one
 * --and plus c would always be a multiple of 3 which is obviously not the case.
 * --
 * --The approach from this mathoverflow post will not suffice
 * 
 * This is very similar to Euclid's formula for Pythagorean triples.
 * It will not generate all triples, but if u and v are coprime we can add
 * a multiplier to the equation and make it generate everything:
 * c = k*(u**2 + 3v**2), a = k*(u**2 + 2uv - 3v**2), b = k*(-u**2 + 2uv + 3v**2)
 * Plugging these into the equation c**2 = a**2 + b**2 - ab:
 * u**4 + 6u**2*v**2 + 9v**4 = u**4 + 4u**2*v**2 + 9v**4 + 4u**3*v - 6u**2*v**2 - 12uv**3 + u**4 + 4u**2*v**2 + 9v**4 - 4u**3*v - 6u**2*v**2 + 12uv**3 
 * 
 * Now the incircle radius r=T/s where T is the area of the triangle and s is its semiperimeter
 * According to Heron's formula, T=sqrt(s(s-a)(s-b)(s-c)), and so we can rearrange the inequality we get
 * to get the inequality
 * 
 * u**6-7u**4*v**2-21u**2*v**4-4u**5*v-36u*v**5+40u**3*v**3+27v**6 <= 2/3*n**2*(u**2+4uv+3v**2)
 * 
 * Considering the boundary, we see that v=-u and v=-u/3 lead to the lhs being zero, meaning these are asymptotes.
 * 
 * We could divide out by these solutions to get a degree 4 equation, change variables so the rhs is a circle,
 * try to eliminate the cross term, or so on.  One thing I noticed was replacing the rhs with (3/2)**6 leads to a boundary
 * which is the same asymptotically (far from the origin in this case) and also looks close to the lines and hyperbola like
 * parts of the boundary.
 * 
 * Actually what I did next was looked at the factorization of the inequality.  I got it by multiplying out:
 * (u**2-4uv+3v**2)(u+3v)(u-3v)(u-v)(u+v)<=2/3*n**2*(u**2+4uv+3v**2)
 * 
 * And from this we can see where the asymptotes we found come from,
 * but also the quadratics factor again:
 * (u-3v)**2*(u-v)**2*(u+3v)(u+v)<=2/3*n**2*(u+3v)(u+v)
 * 
 * And we know u and v are both positive by the constraints that a and b are positive, so we can divide through by (u+3v)(u+v):
 * (u-3v)**2*(u-v)<=2/3*n**2
 * 
 * And now we can take the square root of both sides:
 * |(u-3v)(u-v)|<=sqrt(2/3)n
 * 
 * And the negative part of this inequality corresponds to the part of the boundary within the positive a, b region.
 * 
 * Now just for an estimate, what is the area of the region in uv space bounded by
 * a: (u-3v)(u+v)<=-1
 * b: (x+3y)(x-y)>=1
 * c: -(u-3v)(u-v)<=sqrt(2/3)n
 * 
 * Notice
 * a: v = 1/3*u + 1/3*sqrt(4u**2-3)
 * b: v = -1/3*u + 1/3*sqrt(4u**2+3)
 * c: v = 2/3*u +- 1/3*sqrt(u**2-sqrt(6)n)
 * 
 * a and b intersect at u=sqrt(sqrt(3)/2), v=1/sqrt(2sqrt(3))
 * when n>sqrt(6), c does not intersect with a or b
 * so there should be two integrals:
 * 
 * integral from sqrt(sqrt(3)/2) to sqrt(sqrt(6)n) of a - b
 * integral from sqrt(sqrt(6)n) to infinity of a - c(+) + c(-) - b
 * 
 * integral from sqrt(sqrt(3)/2) to sqrt(sqrt(6)n) of 2/3*u + 1/3*sqrt(4u**2-3) - 1/3*sqrt(4u**2+3)
 */

int main(void){
	
}

