#include <stdio.h>

int inTri(double x, double y, double x1, double y1, double x2, double y2, double x3, double y3){
	x -= x3; x1 -= x3; x2 -= x3;
	y -= y3; y1 -= y3; y2 -= y3;
	y3 = (y*x1 - x*y1)/(y2*x1 - x2*y1);
	if(y3 < 0){
		return 0;
	}
	x3 = (x - x2*y3)/x1;
	if(x3 < 0){
		return 0;
	}
	return 1 > x3 + y3;
}

int main(void){
	FILE *file = fopen("triangles.txt", "r");
	if(!file){
		puts("could not open file.");
		return 1;
	}
	double x = 0, y = 0, x1, y1, x2, y2, x3, y3;
	int c = 0;
	while(fscanf(file, "%lf,%lf,%lf,%lf,%lf,%lf", &x1, &y1, &x2, &y2, &x3, &y3) > 0){
		if(inTri(x, y, x1, y1, x2, y2, x3, y3)){
			++c;
		}
	}
	printf("%i\n", c);
}

