#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "heap.h"

#define I 3
#define J 3

typedef struct{
	double down, left, size;
	uint32_t i_right, i_up;
} box;

int cmp_boxes(const void *_a, const void *_b){
	const box *a = _a, *b = _b;
	if(a->size > b->size){
		return -1;
	}else if(a->size < b->size){
		return 1;
	}
	return 0;
}

uint64_t ncr(uint64_t n, uint64_t r){
	uint64_t p = 1;
	for(uint64_t i = 1; i <= r; ++i, --n){
		p = p*n/i;
	}
	return p;
}

int main(void){
	heap_ft ft = {.cmp=cmp_boxes};
	heap *B = heap_new(256, sizeof(box));
	if(!B){
		return 1;
	}
	heap_push_unordered(B, &(box){.left=1., .size=0.61803398874989484820458683436563811772});
	uint64_t N = ncr(I + J, I);
	uint64_t n = 1;
	for(uint64_t c = 0; c < N; ++n){
		box cur = *(box*)heap_top(B);
		//printf("%"PRIu64": (%"PRIu32", %"PRIu32") (%Lf, %Lf) %.17Lf\n", n, cur.i_right, cur.i_up, cur.left, cur.down, cur.size);
		if(cur.i_right == I && cur.i_up == J){
			++c;
			printf("(%"PRIu64"/%"PRIu64"): %"PRIu64"\n", c, N, n);
			if(c == N){
				break;
			}
		}
		{
			double b = cur.down - cur.left + cur.size;
			box *t = heap_top(B);
			*t = (box){
				.down= cur.down + cur.size,
				.left= cur.left,
				.size= (-b + hypot(b, 2))/2 - cur.left,
				.i_right= cur.i_right,
				.i_up= cur.i_up + 1
			};
			heap_sift_down(B, t, &ft);
		}
		{
			double b = cur.left - cur.down + cur.size;
			if(!heap_push(B, &(box){
				.down= cur.down,
				.left= cur.left + cur.size,
				.size= (-b + hypot(b, 2))/2 - cur.down,
				.i_right= cur.i_right + 1,
				.i_up= cur.i_up
			}, &ft)){
				heap_delete(B, &ft);
				fprintf(stderr, "\e[1;31mERROR: Out of memory (%"PRIu64": %f).\e[0m\n", n, cur.size);
				return 1;
			}
		}
	}
	heap_delete(B, &ft);
	printf("%"PRIu64"\n", n);
}

