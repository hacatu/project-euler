#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct{
	double x, y, m;
} Intersection;

void nextIntersection(Intersection *p){
	/* 4*x^2 + y^2 = 100
	 * 
	 * y - py = m*(x - px)
	 * 
	 * 4*x^2 + (m*x - m*px + py)^2 = 100
	 * 4*x^2 + m^2*x^2 + 2*m*(py - m*px)*x + (py - m*px)^2 = 100
	 * 0 = (4 + m^2)*x^2 +2*m*(py - m*px)*x + (py - m*px)^2 - 100
	 * d = py - m*px
	 * a = 4 + m^2
	 * b = 2*m*d
	 * c = 100 - d^2
	 * x = (-b +- sqrt(b^2 + 4*a*c))/(2*a)
	 * y = m*(x - px) + py
	 * 
	 * mr = (2*mn + m*mn^2 - m)/(2*mn*m - mn^2 + 1)
	 * 
	 * This formula can be derived rather easily from the tangent sum formula, just look up reflecting a line over a line.
	 */
	 double d = p->m*p->x - p->y;
	 double a = 4 + p->m*p->m;
	 double b = 2*p->m*d;
	 double c = 100 - d*d;
	 d = b/(2*a);
	 if(p->x > d){
		 d -= sqrt(b*b + 4*a*c)/(2*a);
	 }else{
		 d += sqrt(b*b + 4*a*c)/(2*a);
	 }
	 p->y = p->m*(d - p->x) + p->y;
	 p->x = d;
	 double mn = p->y/(4*p->x);
	 p->m = (2*mn + p->m*mn*mn - p->m)/(2*mn*p->m - mn*mn + 1);
}

int main(void){
	clock_t start = clock();
	Intersection p = {0, 10.1, (-9.6 - 10.1)/1.4};
	int c = -1;
	do{
		nextIntersection(&p);
		++c;
	}while(p.y < 9.99997);//9.99997999998
	printf("%i %fs\n", c, (double)(clock() - start)/CLOCKS_PER_SEC);
}

