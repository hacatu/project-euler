#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <hac_avl.h>

#define PI 3.1415926535897932384626433832795L
#define EPSILON 0.000000001

/* M______A______O
 *  \    / \    /
 *   \  /   \  /
 *    \/_____\/
 *    C\     /B
 *      \   /
 *       \ /
 *        N
 * C: (0, 0)
 * B: (a, 0)
 * A: (b, 0) rotated by C
 * M: (1/2*b, sqrt(3)/2*b) rotated by C
 * N: (1/2*a, -sqrt(3)/2*a)
 * 
 * T: intersection of circumcircles for MAC and NBC besides C
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

void intersection(long double x1, long double y1, long double r1, long double x2, long double y2, long double r2, long double *x, long double *y){
	x2 -= x1;
	y2 -= y1;
	long double d = hypotl(x2, y2), l = (r1 - r2 + d*d)/(2*d), h = sqrtl(r1 - l*l);
	*x = (l*x2 - h*y2)/d + x1;
	*y = (l*y2 + h*x2)/d + y1;
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
				//printf("%Lf, %Lf, %Lf\n", a, b, c);
				long double n = a*a + b*b - c*c, h = 2*a*b;
				long double sC = sqrtl(h*h - n*n)/h, cC = n/h;
				long double ax = b*cC, ay = b*sC,
				             bx = a, by = 0,
				             cx = 0, cy = 0,
				             mx = b*cC/2 - b*sC/(2*sqrtl(3)), my = b*sC/2 + b*cC/(2*sqrtl(3)), mr = b*b/3,
				             nx = a/2, ny = -a/(2*sqrtl(3)), nr = a*a/3,
				             tx, ty;
				intersection(mx, my, mr, nx, ny, nr, &tx, &ty);
				/*
				printf("A: %Lf, %Lf\n", ax, ay);
				printf("N: %Lf, %Lf\n", nx, ny);
				printf("M: %Lf, %Lf\n", mx, my);
				printf("B: %Lf, %Lf\n", bx, by);
				printf("T: %Lf, %Lf\n", tx, ty);
				*/
				long double da = hypotl(ax - tx, ay - ty);
				long double db = hypotl(bx - tx, by - ty);
				long double dc = hypotl(cx - tx, cy - ty);
				//printf("%Lf, %Lf, %Lf\n", da, db, dc);
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

