#define _POSIX_C_SOURCE 201805L
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>

//ABCD is a quadrilateral with A at (a, 0), B at (0, b), C at (-c, 0), and D at (0, -d).
//1 <= a, b, c, d <= m are integers.
//How many ABCD strictly contain a square number of lattice points for m = 100?

//In an axis aligned right triangle with height a and width b, the area is ab/2.
//The number of points on the hypotenuse is gcd(a, b) + 1.
//There are (a + c + 1)*(b + d + 1) points in the axis aligned bounding rectangle including edge points.
//There are gcd(a, b) + gcd(b, c) + gcd(c, d) + gcd(d, a) total points on the edges of ABCD.
//So we could divide the total number of points by 2 but that would mess up counting the edge points.
//There are 3*(a + b + c + d + 2) - 9 points on edges of the quadrants of the axis aligned bounding rectangle,
//but we only need to count a + b + c + d + 1 of them.  So now there are
//((a + c + 1)*(b + d + 1) - (gcd(a, b) + gcd(b, c) + gcd(c, d) + gcd(d, a)) - (a + b + c + d - 5))/2
//points strictly contained in ABCD.  I think there's few enough rectangles (100000000) that we don't
//necessarily need to do anything smart.  What I'll do is pick b and d first, have gcd cached, compute the
//points in every half quadrilateral for a, pick a, and see how many b's lead to a square.

int is_square(uint64_t n){
	switch(n&0x3Fu){
	case 0x00: case 0x01: case 0x04: case 0x09: case 0x10: case 0x11:
	case 0x19: case 0x21: case 0x24: case 0x29: case 0x31: case 0x39:
	{
		uint64_t r = sqrt(n);
		return r*r == n;
	}
	}
	return 0;
}

uint64_t gcd(uint64_t a, uint64_t b){
	if(a < b){
		b %= a;
	}
	while(1){
		if(!b){
			return a;
		}
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
	}
}

uint64_t count_bruteforce(uint64_t m){
	uint64_t ret = 0;
	uint64_t gcd_memo[m + 1][m + 1];
	for(uint64_t a = 1; a <= m; ++a){
		for(uint64_t b = 1; b <= m; ++b){
			gcd_memo[a][b] = gcd(a, b);
		}
	}
	for(uint64_t b = 1; b <= m; ++b){
		for(uint64_t d = 1; d <= m; ++d){
			for(uint64_t a = 1; a <= m; ++a){
				for(uint64_t c = 1; c <= m; ++c){
					uint64_t n = ((a + c)*(b + d) - (gcd_memo[a][b] + gcd_memo[a][d] + gcd_memo[c][d] + gcd_memo[c][b]) + 2)>>1;
					if(is_square(n)){
						++ret;
					}
				}
			}
		}
	}
	return ret;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Invalid invocation: please use like:\n%s <max radial length>\n", argv[0]);
		exit(0);
	}
	char *m_str_end;
	errno = 0;
	uint64_t m = strtoull(argv[1], &m_str_end, 10);
	if(errno || !m || m >= 0x10000ull){
		printf("The specified max radial length is out of range\n");
		exit(0);
	}else if(*m_str_end){
		printf("The max radial length argument is not an integer\n");
		exit(0);
	}
	uint64_t total_quads = m*m*m*m;
	struct timespec start_time, end_time; 
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	uint64_t squarea_quads = count_bruteforce(m);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	printf(
		"Counted %"PRIu64"/%"PRIu64" quadrilaterals with max radial length %"PRIu64
		" strictly containing a square number of lattice points in %fs\n",
		squarea_quads, total_quads, m,
		end_time.tv_sec + 1e-9*end_time.tv_nsec - start_time.tv_sec - 1e-9*start_time.tv_nsec
	);
}

