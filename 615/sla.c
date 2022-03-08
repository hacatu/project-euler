#include <stdlib.h>
#include "sla.h"

slab_allocator *sla_new(size_t elem_size, size_t elems_cap){
	slab_allocator *ret = malloc(1*sizeof(slab_allocator));
	if(!ret){
		return NULL;
	}
	if(!sla_init(ret, elem_size, elems_cap)){
		free(ret);
		return NULL;
	}
	return ret;
}

int sla_init(slab_allocator *sla, size_t elem_size, size_t elems_cap){
	*sla = (slab_allocator){NULL, NULL, 1, elems_cap, elems_cap, elems_cap, elem_size};
	void *slab = malloc(elems_cap*elem_size);//start the allocator with one block of length elems_cap
	if(!slab){
		return 0;
	}else if(!(sla->elems = malloc(elems_cap*sizeof(void*)))){//create a stack with a pointer to every elem_size offset in slab
		free(slab);
		return 0;
	}else if(!(sla->slabs = malloc(1*sizeof(void*)))){//put slab in an array
		free(slab);
		free(sla->elems);
		return 0;
	}
	sla->slabs[0] = slab;
	for(size_t i = 0; i < elems_cap; ++i){//fill the stack with the pointers
		sla->elems[i] = slab + i*sla->elem_size;
	}
	return 1;
}

void sla_delete(slab_allocator *sla){
	for(size_t i = 0; i < sla->slabs_len; ++i){
		free(sla->slabs[i]);
	}
	free(sla->slabs);
	free(sla->elems);
}

void *sla_alloc(slab_allocator *sla){
	if(sla->elems_len == 0){
		size_t slab_size = sla->slab_size*1.618, elems_cap = sla->elems_cap + slab_size;//1.618 because the golden ratio is the best ratio
		void *slab = malloc(slab_size*sla->elem_size), **elems, **slabs;
		if(!slab){
			return NULL;
		}else if(!(elems = realloc(sla->elems, elems_cap*sizeof(void*)))){
			free(slab);
			return NULL;
		}else if(!(slabs = realloc(sla->slabs, (sla->slabs_len + 1)*sizeof(void*)))){
			free(slab);
			if((elems = realloc(sla->elems, sla->elems_cap*sizeof(void*)))){//although if shrinking with realloc fails ... good luck
				sla->elems = elems;
			}
			return NULL;
		}
		slabs[sla->slabs_len++] = slab;
		sla->slabs = slabs;
		for(size_t i = 0; i < elems_cap - sla->elems_cap; ++i){
			elems[i] = slab + i*sla->elem_size;
		}
		sla->elems = elems;
		sla->slab_size = slab_size;
		sla->elems_len = elems_cap - sla->elems_cap;
		sla->elems_cap = elems_cap;
	}
	return sla->elems[--sla->elems_len];
}

void sla_free(slab_allocator *sla, void *p){
	sla->elems[sla->elems_len++] = p;//Can't overflow if only un- sla_free 'd sla_alloc 'd pointers are passed in
}

