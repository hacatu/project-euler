#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>
#include <inttypes.h>

typedef struct{
	void *buf;
	size_t len, cap, size;
} heap;

typedef struct{
	int (*cmp)(const void*, const void*);
	void (*del)(void*, void*);
	void *del_data;
} heap_ft;

int heap_init(heap *self, size_t cap, size_t size);
heap *heap_new(size_t cap, size_t size);
void heap_ify(heap *self, heap_ft *ft);
size_t heap_size(heap *self);
void *heap_top(heap *self);
void heap_sift_up(heap *self, void *e, heap_ft *ft);
void heap_sift_down(heap *self, void *e, heap_ft *ft);
int heap_push_unordered(heap *self, void *e);
int heap_pop_unordered(heap *self);
int heap_push(heap *self, void *e, heap_ft *ft);
int heap_pop(heap *self, heap_ft *ft);
int heap_trim(heap *self);
void heap_empty(heap *self, heap_ft *ft);
void heap_clear(heap *self, heap_ft *ft);
void heap_delete(heap *self, heap_ft *ft);

#endif

