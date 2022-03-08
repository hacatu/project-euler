#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#define MAX_PERIM 10000000

static int cmp_pts_ab(const void *_a, const void *_b){
	const uint64_t *a = *(const uint64_t(*)[3])_a, *b = *(const uint64_t(*)[3])_b;
	if(a[0] < b[0]){
		return -1;
	}else if(a[0] > b[0]){
		return 1;
	}else if(a[1] < b[1]){
		return -1;
	}else if(a[1] > b[1]){
		return 1;
	}
	return 0;
}

inline static int is_square(uint64_t n){
	uint64_t r = sqrtl(n);
	return n == r*r;
}

uint64_t (*gen_pythag(uint64_t h, size_t *_len))[3]{
	size_t cap = (logl(h/5.L + .5L) + 0.5772156649015329L)*h*M_1_PIl, len = 0;
	uint64_t (*buf)[3] = malloc(cap*sizeof(uint64_t[3]));
	if(!buf){
		return NULL;
	}
	uint64_t i = sqrtl(2.L*h)/2 + 1, j = 0, a, b, c;
	uint64_t (*mns)[2] = malloc(i*sizeof(uint64_t[2]));
	if(!mns){
		free(buf);
		return NULL;
	}
	uint8_t *z = calloc(i, sizeof(uint8_t));
	if(!z){
		free(buf);
		free(mns);
		return NULL;
	}
	mns[0][0] = 2; mns[0][1] = 1;
	while(1){
		while(1){
			a = mns[j][0]*mns[j][0];
			b = mns[j][1]*mns[j][1];
			c = a + b;
			if(c > h){
				break;
			}
			a -= b;
			b = mns[j][0]*mns[j][1] << 1;
			for(uint64_t A = a, B = b, C = c; C <= h; A += a, B += b, C += c){
				memcpy(buf[len++], (uint64_t[3]){A, B, C}, sizeof(uint64_t[3]));
				memcpy(buf[len++], (uint64_t[3]){B, A, C}, sizeof(uint64_t[3]));
			}
			//printf("%"PRIu64"\n", c);
			mns[j + 1][0] = (mns[j][0] << 1) - mns[j][1];
			mns[j + 1][1] = mns[j][0];
			z[j++] = 0;
		}
		while(z[j - 1] == 2){
			if(!--j){
				free(mns);
				free(z);
				*_len = len;
				return realloc(buf, len*sizeof(uint64_t[3])) ?: buf;
			}
		}
		if(z[j - 1]++){
			mns[j][0] = mns[j - 1][0] + (mns[j - 1][1] << 1);
			mns[j][1] = mns[j - 1][1];
		}else{
			mns[j][0] = (mns[j - 1][0] << 1) + mns[j - 1][1];
		}
	}
}

int main(void){
	size_t len = 0;
	uint64_t (*pts)[3] = gen_pythag(MAX_PERIM >> 1, &len), s = 0;
	if(!pts){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	qsort(pts, len, sizeof(uint64_t[3]), cmp_pts_ab);
	for(size_t i = 0, n; i < len; ++i){
		uint64_t b_2 = pts[i][0];
		for(n = i + 1; n <= len && pts[n][0] == b_2; ++n);
		for(; i < n - 1; ++i){
			for(size_t j = n - 1;; --j){
				uint64_t a = pts[j][1] - pts[i][1];
				if(a < pts[i][1]){
					break;
				}
				if(is_square(a*a + (b_2*b_2 << 2))){
					uint64_t p = (a + b_2 + pts[i][2]) << 1;
					if(p <= MAX_PERIM){
						//printf("%"PRIu64",%"PRIu64",%"PRIu64"\n", a, b_2 << 1, pts[i][1]);
						s += p;
					}
				}
			}
		}
	}
	free(pts);
	printf("%"PRIu64"\n", s);
}

