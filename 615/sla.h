#ifndef __SLA_H__
#define __SLA_H__

#include <stddef.h>

typedef struct{
	void **slabs, **elems;//slabs is an array of pointers to exponentially growing blocks; elems is an array used as a stack of pointers into them
	size_t slabs_len, slab_size, elems_len, elems_cap, elem_size;
} slab_allocator;//slab allocator: aggregates small, single struct allocations to decrease dynamic memory requests.

slab_allocator *sla_new(size_t elem_size, size_t elems_cap);
int sla_init(slab_allocator *sla, size_t elem_size, size_t elems_cap);
void sla_delete(slab_allocator *sla);
void *sla_alloc(slab_allocator *sla);
void sla_free(slab_allocator *sla, void *p);

#endif

