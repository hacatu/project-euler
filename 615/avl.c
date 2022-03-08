#include <stdlib.h>
#include <string.h>
#include "avl.h"

#ifdef DEBUG
#include <assert.h>
#include "avl_check.h"
#define ASSERT_LINKS(n) assert(avl_check_links(n))
#define ASSERT_BALANCE(n) assert(avl_check_balance(n) != -1)
#define ASSERT_ALL(n) do{ASSERT_LINKS(n);ASSERT_BALANCE(n);}while(0)
#else
#define ASSERT_LINKS(n)
#define ASSERT_BALANCE(n)
#define ASSERT_ALL(n)
#endif

static avl_node *avl_insert_recursive(avl_node *r, void *key, avl_ft *ft);
inline static avl_node *avl_insert_rebalance_r(avl_node *p, avl_node *n);
inline static avl_node *avl_insert_rebalance_l(avl_node *p, avl_node *n);
static avl_node *avl_insert_retrace(avl_node *n);
inline static avl_node *avl_remove_rebalance_r(avl_node *p);
inline static avl_node *avl_remove_rebalance_l(avl_node *p);
static avl_node *avl_remove_retrace(avl_node *n);
inline static avl_node *avl_remove_trunk(avl_node *n, avl_ft *ft);
inline static avl_node *avl_rotate_r(avl_node *n);
inline static avl_node *avl_rotate_l(avl_node *n);

avl_node *avl_next(avl_node *n){
	if(n->right){
		return avl_first(n->right);
	}
	avl_node *s = n;//inorder successor of n
	while(s->parent && s->parent->left != s){
		s = s->parent;
	}
	return s->parent;
}

avl_node *avl_prev(avl_node *n){
	if(n->left){
		return avl_last(n->left);
	}
	avl_node *p = n;//inorder predecessor of n
	while(p->parent && p->parent->right != p){
		p = p->parent;
	}
	return p->parent;
}

avl_node *avl_new(void *key, avl_node *left, avl_node *right, avl_node *parent, char balance, avl_ft *ft){
	avl_node *ret = ft->alloc(ft->alloc_data);
	if(ret){
		*ret = (avl_node){left, right, parent, balance};
		memcpy(ret->data, key, ft->size);
	}
	return ret;
}

avl_node *avl_root(avl_node *n){
	while(n && n->parent){
		n = n->parent;
	}
	return n;
}

avl_node *avl_get(avl_node *r, void *key, avl_ft *ft){
	if(!r){
		return NULL;
	}
	int ord = ft->cmp(r->data, key);
	if(ord < 0){
		return avl_get(r->right, key, ft);
	}else if(ord > 0){
		return avl_get(r->left, key, ft);
	}//otherwise r->data "==" key
	return r;
}

avl_node *avl_lower_bound(avl_node *r, void *key, avl_ft *ft){
	avl_node *ret = NULL;
	if(!r){
		return NULL;
	}//find largest element l of r such that l <= key
	int ord = ft->cmp(r->data, key);
	if(ord < 0){
		ret = avl_lower_bound(r->right, key, ft);
		return ret ? ret : r;
	}else if(ord > 0){
		return avl_lower_bound(r->left, key, ft);
	}//otherwise r->data "==" key
	return r;
}

avl_node *avl_upper_bound(avl_node *r, void *key, avl_ft *ft){
	avl_node *ret = NULL;
	if(!r){
		return NULL;
	}//find smallest element u of r such that r > key
	int ord = ft->cmp(r->data, key);
	if(ord < 0){
		return avl_upper_bound(r->right, key, ft);
	}else if(ord > 0){
		ret = avl_upper_bound(r->left, key, ft);
		return ret ? ret : r;
	}
	return NULL;
}

avl_node *avl_first(avl_node *r){
	while(r && r->left){
		r = r->left;
	}
	return r;
}

avl_node *avl_last(avl_node *r){
	while(r && r->right){
		r = r->right;
	}
	return r;
}

int avl_insert(avl_node **r, void *key, avl_ft *ft){
	avl_node *t = avl_insert_recursive(*r, key, ft);
	if(t){
		*r = t;
	}
	return !!t;
}

avl_node *avl_insert_recursive(avl_node *r, void *key, avl_ft *ft){
	if(!r){
		return avl_new(key, NULL, NULL, NULL, 0, ft);
	}
	int ord = ft->cmp(r->data, key);
	if(ord < 0){
		if(r->right){
			return avl_insert_recursive(r->right, key, ft);
		}//otherwise insert on the right side
		r->right = avl_new(key, NULL, NULL, r, 0, ft);
		if(!r->right){//allocation failed
			return NULL;
		}
		return avl_insert_retrace(r->right);//rebalance
	}else if(ord > 0){
		if(r->left){
			return avl_insert_recursive(r->left, key, ft);
		}//otherwise insert on the left side
		r->left = avl_new(key, NULL, NULL, r, 0, ft);
		if(!r->left){//allocation failed
			return NULL;
		}
		return avl_insert_retrace(r->left);//rebalance
	}
	return NULL;
}

int avl_remove(avl_node **r, void *key, avl_ft *ft){
	avl_node *n = avl_get(*r, key, ft);
	if(n){
		*r = avl_remove_node(n, ft);
	}
	return !!n;
}

avl_node *avl_remove_node(avl_node *n, avl_ft *ft){
	avl_node *s = avl_next(n);
	if((!n->left) || (!n->right)){//no need to swap with a scapegoat
		return avl_remove_trunk(n, ft);
	}
	//swap all of n and s's parent, left, right, and balance fields
	//it would be easier to swap the data fields but that would invalidate iterators to s
	avl_node t = *s;//t is an automatic storage variable that holds a copy of *s
	if((s->left = n->left)){
		s->left->parent = s;
	}
	if((s->right = n->right)){
		s->right->parent = s;
	}
	if((s->parent = n->parent)){
		*(s->parent->left == n ? &s->parent->left : &s->parent->right) = s;//dereference(reference) allows the ternary operator to be an lvalue
	}
	s->balance = n->balance;
	n->left = NULL;//s is n's inorder successor and n has a right child so s does not have a left child.
	if((n->right = t.right)){
		n->right->parent = n;
	}
	if((n->parent = t.parent)){
		*(n->parent->left == s ? &n->parent->left : &n->parent->right) = n;
	}
	n->balance = t.balance;
	if(t.parent == n){//s was n's child.  In this case, these pointers loop back and need to be fixed.
		s->right = n;
		n->parent = s;
		n->right = t.right;
	}
	return avl_remove_trunk(n, ft);
}

avl_node *avl_remove_trunk(avl_node *n, avl_ft *ft){
	avl_node *p = n->parent, *c = n->left ? n->left : n->right, *r;//Parent, Child
	if(!p){
		if(c){
			c->parent = NULL;
		}
		ft->free(ft->alloc_data, n);
		return c;//This does not indicate an error if NULL.
	}
	if(p->left == n){
		r = avl_remove_retrace(n);//r is the root now
		n->parent->left = c;
		if(c){
			c->parent = n->parent;
		}
		ft->free(ft->alloc_data, n);
		return r;
	}//p->right == n
	r = avl_remove_retrace(n);//r is the root
	n->parent->right = c;
	if(c){
		c->parent = n->parent;
	}
	ft->free(ft->alloc_data, n);
	return r;
}

avl_node *avl_insert_retrace(avl_node *n){
	avl_node *p = n->parent;
	if(!p){
		return n;
	}
	if(n == p->right){//+2 cases
		switch(p->balance){
			case -1://height change absorbed
			p->balance = 0;
			return avl_root(p);
			case 0:
			p->balance = 1;
			return avl_insert_retrace(p);
		}//default: p->balance == 1
		return avl_insert_rebalance_r(p, n);//height change will be absorbed
	}//otherwise n == p->left
	switch(p->balance){//-2 cases
		case 1://height change absorbed
		p->balance = 0;
		return avl_root(p);
		case 0:
		p->balance = -1;
		return avl_insert_retrace(p);
	}//default: p->balance == -1
	return avl_insert_rebalance_l(p, n);//height change will be absorbed
}

avl_node *avl_insert_rebalance_r(avl_node *p, avl_node *n){
	if(n->balance == 1){//right-right case: only one rotation needed
		avl_rotate_r(p);//note n becomes the parent of p
		p->balance = n->balance = 0;
		return avl_root(n);
	}//otherwise right-left case: two rotations are required and the balance factors have 3 cases
	avl_rotate_l(n);
	avl_rotate_r(p);//note p, n->left, n change roles from parent, right child, right-left grandchild to left child, parent, right child
	n->balance = +(n->parent->balance == -1);//n, now the right child of n->parent, its old left child, has a balance factor of 1 if the child was -1, else 0
	p->balance = -(n->parent->balance == 1);//p, now the right child of n->parent, has a balance factor of -1 if n->parent was 1, else 0
	n->parent->balance = 0;//if this is still confusing, try drawing out all of the possible trees with root +2 and right child +/-1 with irrelevant subtrees only marked as heights.
	return avl_root(n->parent);//there are 4 cases and I will try to put them in a pdf somewhere if I can figure out how to draw it on a computer.
}

avl_node *avl_insert_rebalance_l(avl_node *p, avl_node *n){//mirror image of avl_insert_rebalance_r
	if(n->balance == -1){
		avl_rotate_l(p);
		p->balance = n->balance = 0;
		return avl_root(n);
	}
	avl_rotate_r(n);
	avl_rotate_l(p);
	n->balance = -(n->parent->balance == 1);
	p->balance = +(n->parent->balance == -1);//unary + for symmetry
	n->parent->balance = 0;
	return avl_root(n->parent);
}

avl_node *avl_remove_retrace(avl_node *n){
	avl_node *p = n->parent;
	if(!p){
		return n;
	}
	if(n == p->left){//+2 cases
		switch(p->balance){
			case -1:
			p->balance = 0;
			return avl_remove_retrace(p);
			case 0://change in height absorbed
			p->balance = 1;
			return avl_root(p);
		}//default: p->balance == 1
		return avl_remove_rebalance_l(p);
	}//otherwise n == p->right
	switch(p->balance){//-2 cases
		case 1:
		p->balance = 0;
		return avl_remove_retrace(p);
		case 0://change in height absorbed
		p->balance = -1;
		return avl_root(p);
	}//default: p->balance == -1
	return avl_remove_rebalance_r(p);
}

avl_node *avl_remove_rebalance_l(avl_node *p){//do not try to understand this until you understand avl_insert_rebalance_r
	avl_node *n = p->right;//If we've become off balance by a left REMOVAL there must be a right child
	if(n->balance != -1){//not the right-left case
		avl_rotate_r(p);//nb. the pointers n and p point to the same nodes but now n is the parent
		p->balance -= n->balance--;//if n is +1, n and p become 0, otherwise n is 0 and they become -1 and +1
		return n->balance ? avl_root(n) : avl_remove_retrace(n);//if n->balance becomes 0 by removal we still have height decrease to propagate
	}//right-left case.  This is exactly the same as insertion whereas the previous part included a right-even case not present in insertion
	avl_rotate_l(n);
	avl_rotate_r(p);
	p->balance = -(n->parent->balance == 1);
	n->balance = +(n->parent->balance == -1);
	n->parent->balance = 0;
	return avl_remove_retrace(n->parent);
}

avl_node *avl_remove_rebalance_r(avl_node *p){//mirror image of avl_remove_rebalance_l
	avl_node *n = p->left;
	if(n->balance != 1){
		avl_rotate_l(p);
		p->balance -= n->balance++;//if n is -1, n and p become 0, otherwise n is 0 and they become +1 and -1.  Note the -= needn't be flipped since -- was
		return n->balance ? avl_root(n) : avl_remove_retrace(n);
	}
	avl_rotate_r(n);
	avl_rotate_l(p);
	p->balance = +(n->parent->balance == -1);
	n->balance = -(n->parent->balance == 1);
	n->parent->balance = 0;
	return avl_remove_retrace(n->parent);
}

avl_node *avl_rotate_l(avl_node *n){//does not update balance factors because the caller knows better
	avl_node *l = n->left;//assume existence of swapped node
	l->parent = n->parent;
	if(n->parent){
		if(n->parent->left == n){
			n->parent->left = l;
		}else{//n->parent->right == n
			n->parent->right = l;
		}
	}
	n->parent = l;
	n->left = l->right;
	if(n->left){
		n->left->parent = n;
	}
	l->right = n;
	return l;
}

avl_node *avl_rotate_r(avl_node *n){
	avl_node *r = n->right;//assume existence of swapped node
	r->parent = n->parent;
	if(n->parent){
		if(n->parent->left == n){
			n->parent->left = r;
		}else{//n->parent->right == n
			n->parent->right = r;
		}
	}
	n->parent = r;
	n->right = r->left;
	if(n->right){
		n->right->parent = n;
	}
	r->left = n;
	return r;
}

void avl_delete(avl_node *r, avl_ft *ft){
	if(!r)return;
	avl_delete(r->left, ft);
	avl_delete(r->right, ft);
	ft->free(ft->alloc_data, r);
}

