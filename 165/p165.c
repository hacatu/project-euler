#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

typedef long double quad;

quad crossP(quad ax, quad ay, quad bx, quad by){
	return ax*by - ay*bx;
}

int cmpPoints(quad (*a)[2], quad (*b)[2]){
	quad o = (*b)[0] - (*a)[0];
	if(o < 0){
		return -1;
	}else if(o > 0){
		return 1;
	}else{
		return 0;
	}
}

int intersection(quad px, quad py, quad rx, quad ry, quad qx, quad qy, quad sx, quad sy, quad *x, quad *y){
	quad a = crossP(rx, ry, sx, sy);
	if(!a){
		return 0;
	}
	quad dx = (qx - px)/a;
	quad dy = (qy - py)/a;
	quad t = crossP(dx, dy, rx, ry);
	if(t <= 0 || t >= 1){
		return 0;
	}
	t = crossP(dx, dy, sx, sy);
	if(t <= 0 || t >= 1){
		return 0;
	}
	*x = px + t*rx;
	*y = py + t*ry;
	return 1;
}

int main(void){
	quad *t = malloc(20000*sizeof(quad));
	quad (*is)[2] = malloc(5000*5001/2*sizeof(quad[2]));
	uint64_t ic = 0, c = 0;
	for(uint64_t s = (uint64_t)290797*290797%50515093, n = 0; n < 20000; ++n, s = s*s%50515093){
		t[n] = s%500;
	}
	for(uint64_t i = 0; i < 20000; i += 4){
		t[i + 2] -= t[i];
		t[i + 3] -= t[i + 1];
	}
	for(uint64_t i = 0; i < 19996; i += 4){
		for(uint64_t j = i + 4; j < 20000; j += 4){
			if(intersection(t[i], t[i + 1], t[i + 2], t[i + 3], t[j], t[j + 1], t[j + 2], t[j + 3], &is[ic][0], &is[ic][1])){
				++ic;
			}
		}
	}
	printf("%"PRIu64"\n", ic);
	qsort(is, ic, sizeof(quad[2]), (__compar_fn_t)cmpPoints);
	for(uint64_t i = 0; i < ic; ++i){
		for(uint64_t j = i + 1; j < ic; ++j){
			if(fabsl(is[i][0] - is[j][0]) >= 0.00000001){
				break;
			}
			if(fabsl(is[i][1] - is[j][1]) < 0.00000001){
				goto CONTINUE;
			}
		}
		++c;
		CONTINUE:;
	}
	printf("%"PRIu64"\n", c);
	free(t);
	free(is);
}

