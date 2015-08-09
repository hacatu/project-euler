#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <hac_avl.h>
#include <pthread.h>

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
 * T: intersection of MB and AN
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

void intersection(long double x1, long double y1, long double x2, long double y2, long double x3, long double y3, long double x4, long double y4, long double *x, long double *y){
	long double a = x1*y2 - y1*x2, b = x3*y4 - y3*x4;
	x1 -= x2;
	y1 -= y2;
	x3 -= x4;
	y3 -= y4;
	long double d = x1*y3 - y1*x3;
	*x = (a*x3 - b*x1)/d;
	*y = (a*y3 - b*y1)/d;
}

HAC_AVL_T_NEW(int);

typedef struct{
	long double start, step;
	HAC_AVL_T(int) *perims;
	int *s;
	pthread_mutex_t *avl_ex;
} Arg;

void *thread_fn(void *vp){
	Arg *arg = vp;
	for(long double a = arg->start;; a += arg->step){
		int first_a = 1;
		for(long double b = floor(a/sqrtl(3)) + 1; b <= a + EPSILON; ++b){
			int first_b = 1;
			for(long double c = floor((sqrtl(4*a*a - 3*b*b) - b)/2) + 1; c <= b + EPSILON; ++c){
				//printf("%Lf, %Lf, %Lf\n", a, b, c);
				long double n = (a*a + b*b - c*c)/(2*a*b);
				long double ax = b*n, ay = b*sqrtl(1 - n*n),
				             mx = b/2*(n - sqrtl(3)*sqrtl(1 - n*n)), my = b/2*(sqrtl(1 - n*n) + sqrtl(3)*n),
				             nx = a/2, ny = -a*sqrtl(3)/2,
				             tx, ty;
				intersection(mx, my, a, 0, ax, ay, nx, ny, &tx, &ty);
				/*
				printf("A: %Lf, %Lf\n", ax, ay);
				printf("N: %Lf, %Lf\n", nx, ny);
				printf("M: %Lf, %Lf\n", mx, my);
				printf("B: %Lf, %Lf\n", bx, by);
				printf("T: %Lf, %Lf\n", tx, ty);
				*/
				long double da = hypotl(ax - tx, ay - ty);
				long double db = hypotl(a - tx, ty);
				long double dc = hypotl(tx, ty);
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
					pthread_mutex_lock(arg->avl_ex);
					if(HAC_AVL_INSERT(int, arg->perims, perim, compInts(e1, e2))){
						*arg->s += perim;
					}
					pthread_mutex_unlock(arg->avl_ex);
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
}

int main(void){
	HAC_AVL_T(int) perims = HAC_AVL_EMPTY(int);
	int s = 0;
	int cpus = 8;
	pthread_t threads[cpus];
	pthread_mutex_t avl_ex;
	pthread_mutex_init(&avl_ex, NULL);
	Arg args[cpus];
	for(int i = 0; i < cpus; ++i){
		args[i] = (Arg){.start = i + 1, .step = cpus, .s = &s, .perims = &perims, .avl_ex = &avl_ex};
		if(pthread_create(&threads[i], NULL, thread_fn, &args[i])){
			puts("Failed to create thread.");
			return 1;
		}
	}
	for(int i = 0; i < cpus; ++i){
		if(pthread_join(threads[i], NULL)){
			puts("Failed to join thread");
			return 1;
		}
	}
	HAC_AVL_DELETE(int, &perims);
	printf("%i\n", s);
}

