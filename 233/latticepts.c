#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*(x - N/2)^2 + (y - N/2)^2 = N^2/2
  u = 2*x - N
  v = 2*y - N
  u^2 + v^2 = 2*N^2
  this function searches for lattice points on the circle through
  (0, 0), (N, 0), (0, N), (N, N) on the top arc from (0, N) inclusive to
  (N, N) exclusive.
*/
int64_t latpts_xy_brute(int64_t N){
	int64_t c = 0;
	for(int64_t x = 0; x < N; ++x){
		/*(y - N/2)^2 = N^2/2 - (x - N/2)^2
		  (2*y - N)^2 = 2*N^2 - (2*x - N)^2
		  y = (sqrt(2*N^2 - (2*x - N)^2) + N)/2
		*/
		int64_t y = (sqrt(2*N*N - (2*x - N)*(2*x - N)) + N)/2;
		if((2*x - N)*(2*x - N) + (2*y - N)*(2*y - N) == 2*N*N){
			printf("xy: (%"PRIi64",%"PRIi64")\n", x, y);
			++c;
		}
	}
	return 4*c;
}

int64_t latpts_uv_brute(int64_t N){
	int64_t c = 0;
	for(int64_t u = 0; u*u < 2*N*N; ++u){
		/*u^2 + v^2 = 2*N^2
		  v = sqrt(2*N^2 - u^2)
		*/
		int64_t v = sqrt(2*N*N - u*u);
		if(u*u + v*v == 2*N*N){
			int64_t x = (u + N)/2, y = (v + N)/2;
			if(x >= N){
				//x, y = N - y, x
				int64_t t = x;
				x = N - y;
				y = t;
			}
			printf("uv: (%"PRIi64",%"PRIi64") -> xy: (%"PRIi64",%"PRIi64")\n", u, v, x, y);
			++c;
		}
	}
	return 4*c;
}

int main(int argc, char **argv){
	int64_t n = latpts_xy_brute(10000);
	printf("Fount %"PRIi64" lattice points for N=10000 by brute force.\n", n);
	n = latpts_uv_brute(10000);
	printf("Found %"PRIi64" lattice points for N=10000 by brute force on the transformed domain.\n", n);
}

