#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "heap.h"

void initb(uint8_t *buf, uint64_t i){
		buf[0] = 1;
		buf[1] = buf[0] + !(i&1);
		buf[2] = buf[0] + !(i&2);
		buf[3] = buf[1] + !(i&4);
		buf[4] = buf[1] + !(i&8);
		buf[5] = buf[2] + !(i&16);
		buf[6] = buf[2] + !(i&32);
}

int cmp_uint8_ts(const void *_a, const void *_b){
	const uint8_t *a = _a, *b = _b;
	if(*a < *b){
		return -1;
	}else if(*a > *b){
		return 1;
	}
	return 0;
}

heap_ft ft = {.cmp=cmp_uint8_ts};

int heap_check(const heap *self, uint64_t i, heap_ft *ft){
	uint64_t l = (i << 1) + 1, r = l + 1;
	if(l < self->len){
		if(r < self->len){
			if(!heap_check(self, r, ft)){
				return 0;
			}if(ft->cmp(self->buf + i*self->size, self->buf + r*self->size) > 0){
				return 0;
			}
		}if(!heap_check(self, l, ft)){
			return 0;
		}if(ft->cmp(self->buf + i*self->size, self->buf + l*self->size) > 0){
			return 0;
		}
	}
	return 1;
}

int main(void){
	heap h;
	if(!heap_init(&h, 15, sizeof(uint8_t))){
		return 1;
	}
	for(uint64_t i = 0; i < 1 << 7; ++i){
		initb(h.buf, i);
		uint8_t t[7];
		memcpy(t, h.buf, 7*sizeof(uint8_t));
		for(uint64_t s = 0; s <= 7; ++s){
			memcpy(h.buf, t, s*sizeof(uint8_t));
			h.len = s;
			uint64_t a = 0;
			heap_push(&h, &a, &ft);
			if(!heap_check(&h, 0, &ft)){
				fprintf(stderr, "\e[1;31mERROR: heap_push: i=%"PRIu64", s=%"PRIu64"\e[0m\n", i, s);
			}
			if(s){
				memcpy(h.buf, t, s*sizeof(uint8_t));
				h.len = s;
				*(uint8_t*)h.buf = s + 2;
				heap_sift_down(&h, h.buf, &ft);
				if(!heap_check(&h, 0, &ft)){
					fprintf(stderr, "\e[1;31mERROR: heap_sift_down: i=%"PRIu64", s=%"PRIu64"\e[0m\n", i, s);
				}
			}
		}
	}
	heap_clear(&h, &ft);
}

