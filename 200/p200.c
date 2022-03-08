#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct{
	uint64_t head, prime;
} multiple_generator_t;

int cmp_multiple_generators(const void *a, const void *b){
	if(((multiple_generator_t*)a)->head < ((multiple_generator_t*)b)->head){
		return -1;
	}else if(((multiple_generator_t*)a)->head > ((multiple_generator_t*)b)->head){
		return 1;
	}
	return 0;
}

typedef struct{
	void *root;
	size_t len, cap, size;
	int (*cmp)(const void*, const void*);
	void (*free)(void*);
} heap_t;

int f(uint64_t n){
	return __builtin_clzll(n);
}

int heap_init(heap_t *heap, size_t size, size_t cap, int (*cmp)(const void*, const void*), void (*free)(void*)){
	cap = (1 << (64 - __builtin_clzll(cap))) - 1;
	return !!(*heap = (heap_t){malloc(size*cap), 0, cap, size, cmp, free}).root;
}

void *heap_top(heap_t *heap){
	return heap->root;
}

void heap_sift_up(heap_t *heap, void *a){
	size_t i = (a - heap->root)/heap->size;
	size_t j = i>>1;
	char tmp[heap->size];
	while(i && heap->cmp(heap->root + i*heap->size, heap->root + j*heap->size) < 0){
		memcpy(tmp, heap->root + i*heap->size, heap->size);
		memcpy(heap->root + i*heap->size, heap->root + j*heap->size, heap->size);
		memcpy(heap->root + j*heap->size, tmp, heap->size);
		i = j;
		j = i>>1;
	}
}

void heap_sift_down(heap_t *heap, void *a){
	size_t i = (a - heap->root)/heap->size;
	size_t l = (i<<1) + 1, r = l + 1, j;
	char tmp[heap->size];
	while(r < heap->len){
		j = heap->cmp(heap->root + l*heap->size, heap->root + r*heap->size) <= 0 ? l : r;
		if(heap->cmp(heap->root + i*heap->size, heap->root + j*heap->size) <= 0){
			return;
		}
		memcpy(tmp, heap->root + i*heap->size, heap->size);
		memcpy(heap->root + i*heap->size, heap->root + j*heap->size, heap->size);
		memcpy(heap->root + j*heap->size, tmp, heap->size);
		i = j;
		l = (i<<1) + 1, r = l + 1;
	}
	if(l < heap->len && heap->cmp(heap->root + i*heap->size, heap->root + l*heap->size) > 0){
		memcpy(tmp, heap->root + i*heap->size, heap->size);
		memcpy(heap->root + i*heap->size, heap->root + l*heap->size, heap->size);
		memcpy(heap->root + l*heap->size, tmp, heap->size);
	}
}

int heap_insert(heap_t *heap, void *a){
	if(heap->len + 1 > heap->cap){
		void *tmp = realloc(heap->root, (heap->cap*2 + 1)*heap->size);
		if(!tmp){
			return 0;
		}
		heap->root = tmp;
		heap->cap = heap->cap*2 + 1;
	}
	heap_sift_up(heap, memcpy(heap->root + heap->len++*heap->size, a, heap->size));
	return 1;
}

void heap_pop(heap_t *heap){
	memcpy(heap->root, heap->root + --heap->len*heap->size, heap->size);
}

void heap_update(heap_t *heap, void *a){
	heap_sift_down(heap, a);
}

void heap_delete(heap_t *heap){
	if(heap->free){
		for(size_t i = 0; i < heap->size; ++i){
			heap->free(heap->root + i*heap->size);
		}
	}
	free(heap->root);
}

uint64_t next_prime(heap_t *multiple_generators, size_t *n){
	++*n;
	while(1){
		multiple_generator_t *next_composite = heap_top(multiple_generators);
		if(next_composite->head < *n){
			next_composite->head += next_composite->prime;
			heap_update(multiple_generators, next_composite);
		}else if(next_composite->head == *n){
			++*n;
		}else{
			heap_insert(multiple_generators, &(multiple_generator_t){.head=*n**n,.prime=*n});
			return *n;
		}
	}
}

int main(void){
	heap_t multiple_generators;
	heap_init(&multiple_generators, sizeof(multiple_generator_t), 1, cmp_multiple_generators, NULL);
	uint64_t p = 2, s = 0;
	heap_insert(&multiple_generators, &(multiple_generator_t){.head=p*p,.prime=p});
	while(p < 1000000){
		s += p;
		next_prime(&multiple_generators, &p);
	}
	printf("%"PRIu64"\n", s);
	heap_delete(&multiple_generators);
}

