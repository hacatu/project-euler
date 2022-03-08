#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long double cover(size_t n){
	size_t triangles_l = 4;
	for(size_t i = 1; i < n; ++i){
		triangles_l *= 3;
	}
	long double c0 = 3 - 2*sqrtl(3);
	long double (*triangles)[4] = calloc(triangles_l, sizeof(long double [4]));
	memcpy(triangles[0], (long double[]){1, 1, 1, 0}, 4*sizeof(long double));
	memcpy(triangles[1], (long double[]){c0, 1, 1, 0}, 4*sizeof(long double));
	memcpy(triangles[2], (long double[]){c0, 1, 1, 0}, 4*sizeof(long double));
	memcpy(triangles[3], (long double[]){c0, 1, 1, 0}, 4*sizeof(long double));
	size_t triangles_a = 0, triangles_b = 4, triangles_c = 4;
	long double a0 = 1/(c0*c0), a = a0 - 3;
	if(n)
	while(triangles_c){
		long double k1 = triangles[triangles_a][0], k2 = triangles[triangles_a][1], k3 = triangles[triangles_a][2], k4, layer = triangles[triangles_a][3];
		triangles_a = (triangles_a + 1)%triangles_l;
		--triangles_c;
		k4 = k1 + k2 + k3 + 2*sqrtl(k1*k2 + k2*k3 + k3*k1);
		a -= 1/(k4*k4);
		if(++layer == n){
			continue;
		}
		memcpy(triangles[triangles_b], (long double[]){k2, k3, k4, layer}, 4*sizeof(long double));
		triangles_b = (triangles_b + 1)%triangles_l;
		memcpy(triangles[triangles_b], (long double[]){k1, k3, k4, layer}, 4*sizeof(long double));
		triangles_b = (triangles_b + 1)%triangles_l;
		memcpy(triangles[triangles_b], (long double[]){k1, k2, k4, layer}, 4*sizeof(long double));
		triangles_b = (triangles_b + 1)%triangles_l;
		triangles_c += 3;
	}
	free(triangles);
	return a/a0;
}

int main(void){
	printf("%.8Lf\n", cover(10));
}

