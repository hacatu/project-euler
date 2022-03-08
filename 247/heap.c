#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "heap.h"

int heap_init(heap *self, size_t cap, size_t size){
	cap = (1 << (64 - __builtin_clzll(cap))) - 1;
	*self = (heap){malloc(cap*size), 0, cap, size};
	return !!self->buf;
}

heap *heap_new(size_t cap, size_t size){
	heap *ret = malloc(1*sizeof(heap));
	if(ret){
		if(!heap_init(ret, cap, size)){
			free(ret);
			ret = NULL;
		}
	}
	return ret;
}

void heap_ify(heap *self, heap_ft *ft){
	for(size_t i = self->cap >> 1; i-- > 0;){
		heap_sift_down(self, self->buf + i*self->size, ft);//void pointer arithmetic works like char pointer arithmetic
	}
}

size_t heap_size(heap *self){
	return self->len;
}

void *heap_top(heap *self){
	return self->len ? self->buf : NULL;
}

void heap_sift_up(heap *self, void *e, heap_ft *ft){
	size_t i = (e - self->buf)/self->size;
	size_t j = (i - 1) >> 1;
	char tmp[self->size];
	while(i && ft->cmp(self->buf + i*self->size, self->buf + j*self->size) < 0){
		memcpy(tmp, self->buf + i*self->size, self->size);
		memcpy(self->buf + i*self->size, self->buf + j*self->size, self->size);
		memcpy(self->buf + j*self->size, tmp, self->size);
		i = j;
		j = (i - 1) >> 1;
	}
}

void heap_sift_down(heap *self, void *e, heap_ft *ft){
	size_t i = (e - self->buf)/self->size;
	size_t l = (i << 1) + 1, r = l + 1, j;
	char tmp[self->size];
	while(r < self->len){
		j = ft->cmp(self->buf + l*self->size, self->buf + r*self->size) <= 0 ? l : r;
		if(ft->cmp(self->buf + i*self->size, self->buf + j*self->size) <= 0){
			return;
		}
		memcpy(tmp, self->buf + i*self->size, self->size);
		memcpy(self->buf + i*self->size, self->buf + j*self->size, self->size);
		memcpy(self->buf + j*self->size, tmp, self->size);
		i = j;
		l = (i << 1) + 1, r = l + 1;
	}
	if(l < self->len && ft->cmp(self->buf + i*self->size, self->buf + l*self->size) > 0){
		memcpy(tmp, self->buf + i*self->size, self->size);
		memcpy(self->buf + i*self->size, self->buf + l*self->size, self->size);
		memcpy(self->buf + l*self->size, tmp, self->size);
	}
}

int heap_push_unordered(heap *self, void *e){
	if(self->len + 1 > self->cap){
		void *tmp = realloc(self->buf, ((self->cap << 1) + 1)*self->size);
		if(!tmp){
			return 0;
		}
		self->buf = tmp,
		self->cap = (self->cap << 1) + 1;
	}
	memcpy(self->buf + self->len++*self->size, e, self->size);
	return 1;
}

int heap_push(heap *self, void *e, heap_ft *ft){
	if(!heap_push_unordered(self, e)){
		return 0;
	}
	heap_sift_up(self, self->buf + (self->len - 1)*self->size, ft);
	return 1;
}

int heap_pop_unordered(heap *self){
	if(!self->len){
		return 0;
	}
	if(!--self->len){
		return 1;
	}
	memcpy(self->buf, self->buf + self->len*self->size, self->size);
	return 1;
}

int heap_pop(heap *self, heap_ft *ft){
	if(!self->len){
		return 0;
	}
	if(!--self->len){
		return 1;
	}
	heap_sift_down(self, memcpy(self->buf, self->buf + self->len*self->size, self->size), ft);
	return 1;
}

int heap_trim(heap *self){
	size_t new_cap = (1 << (64 - __builtin_clzll(self->len))) - 1;
	if(new_cap < self->cap){
		void *tmp = realloc(self->buf, new_cap*self->size);
		if(!tmp){
			return 0;
		}
		self->buf = tmp;
		self->cap = new_cap;
	}
	return 1;
}

void heap_empty(heap *self, heap_ft *ft){
	if(ft->del){
		for(size_t i = 0; i < self->len; ++i){
			ft->del(ft->del_data, self->buf + i*self->size);//void pointer arithmentic
		}
	}
	self->len = 0;
}

void heap_clear(heap *self, heap_ft *ft){
	heap_empty(self, ft);
	free(self->buf);
	self->buf = NULL;
	self->cap = 0;
}

void heap_delete(heap *self, heap_ft *ft){
	heap_clear(self, ft);
	free(self);
}

