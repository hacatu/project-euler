#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <hac_avl.h>

#define PI 3.1415926535897932384626433832795L
#define EPSILON 0.000000001

/* Trilinear coordinates for Fermat point:
 * csc(A + pi/3):csc(B + pi/3):csc(C + pi/3)
 * 
 * Actual distance trilinears are such that:
 * a*u + b*v + c*w = 2*t where t is the area
 * 
 * t = sqrt(s*(s - a)*(s - b)*(s - c))
 * 
 * For actual distance trilinears, the distance between two points is:
 * 
 * d = sqrt(u^2 + v^2 + 2*u*v*cos(C)/sin(C)
 */

/*
Constraints (ignoring angles):
a - b <= c <= b
The triangle can have at most 1 angle greater than or equal to 120 and since the largest angle is opposite the largest side this angle is opposite a.
angle(a, b, c) < 120
acos((b^2 + c^2 - a^2)/(2*b*c)) < 2*pi/3
-1/2 < (b^2 + c^2 - a^2)/(2*b*c)
0 < c^2 + b*c + b^2 - a^2
c > (sqrt(4*a^2 - 3*b^2) - b)/2

Constraints for b:
0 < b^2 + c*b + c^2 - a^2
But when b is initialized, c is unknown, so assume it is b:
0 < b^2 + b*b + b^2 - a^2
0 < 3*b^2 - a^2
a/sqrt(3) < b
*/

int isInt(long double n){
	long double r = roundl(n);
	n -= r;
	if(n < 0){
		n = -n;
	}
	return n <= EPSILON;
}

int compInts(int e1, int e2){
	if(e1 > e2){
		return -1;
	}else if(e1 < e2){
		return 1;
	}
	return 0;
}

int main(void){
	HAC_AVL_T_NEW(int);
	HAC_AVL_T(int) perims = HAC_AVL_EMPTY(int);
	int s = 0;
	for(long double a = 1; a < 1000; ++a){
		printf("%Lf\n", a);
		int first_a = 1;
		for(long double b = floor(a/sqrtl(3)) + 1; b <= a + EPSILON; ++b){
			int first_b = 1;
			for(long double c = floor((sqrtl(4*a*a - 3*b*b) - b)/2) + 1; c <= b + EPSILON; ++c){
				long double u = (b*b + c*c - a*a)/(2*b*c);
				u = 2/(sqrtl(1 - u*u) + sqrtl(3)*u);
				long double v = (c*c + a*a - b*b)/(2*c*a);
				v = 2/(sqrtl(1 - v*v) + sqrtl(3)*v);
				long double n = (a*a + b*b - c*c)/(2*a*b), w;
				w = 2/(sqrtl(1 - n*n) + sqrtl(3)*n);
				long double s = (a + b + c)/2;
				long double t = sqrtl(s*(s - a)*(s - b)*(s - c));
				long double k = 2*t/(a*u + b*v + c*w);
				u *= k;
				v *= k;
				long double ha = 2*t/a, hb = 2*t/b;
				//d = sqrt((a1 - a2)^2 + (b1 - b2)^2 + 2*(a1 - a2)*(b1 - b2)*cos(C))/sin(C)
				long double da = sqrtl((u - ha)*(u - ha) + v*v + 2*(u - ha)*v*n)/sqrtl(1 - n*n);
				long double db = sqrtl(u*u + (v - hb)*(v - hb) + 2*u*(v - hb)*n)/sqrtl(1 - n*n);
				long double dc = sqrtl(u*u + v*v + 2*u*v*n)/sqrtl(1 - n*n);
				if(da + db + dc > 120000 + EPSILON){
					if(first_b == 1){
						first_b = 2;
					}
					break;
				}
				first_b = 0;
				if(isInt(da) && isInt(db) && isInt(dc)){
					int perim = roundl(da + db + dc);
					printf("%Lf, %Lf, %Lf, %i\n", a, b, c, perim);
					if(HAC_AVL_INSERT(int, &perims, perim, compInts(e1, e2))){
						s += perim;
					}
				}
			}
			if(first_b == 2){
				if(first_a == 1){
					first_a = 2;
				}
				break;
			}
			first_a = 0;
		}
		if(first_a == 2){
			break;
		}
	}
	HAC_AVL_DELETE(int, &perims);
	printf("%i\n", s);
}

