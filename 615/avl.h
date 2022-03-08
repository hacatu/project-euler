#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>

typedef struct avl_node avl_node;
struct avl_node{
	avl_node *left, *right, *parent;
	signed char balance;
	char data[];
};

typedef struct{//function table to hold function pointers and related data
	size_t size;
	int (*cmp)(const void*, const void*);
	void *(*alloc)(void*);
	void (*free)(void*, void*);
	void *alloc_data;
} avl_ft;

avl_node *avl_new(void *key, avl_node *left, avl_node *right, avl_node *parent, char balance, avl_ft *ft);
int avl_insert(avl_node **r, void *key, avl_ft *ft);
int avl_remove(avl_node **r, void *key, avl_ft *ft);
avl_node *avl_remove_node(avl_node *n, avl_ft *ft);
avl_node *avl_get(avl_node *r, void *key, avl_ft *ft);
avl_node *avl_root(avl_node *n);
avl_node *avl_first(avl_node *r);
avl_node *avl_next(avl_node *n);
avl_node *avl_last(avl_node *n);
avl_node *avl_prev(avl_node *n);
avl_node *avl_lower_bound(avl_node *r, void *key, avl_ft *ft);
avl_node *avl_upper_bound(avl_node *r, void *key, avl_ft *ft);
void avl_delete(avl_node *r, avl_ft *ft);

#endif

