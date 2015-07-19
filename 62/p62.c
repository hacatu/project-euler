#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct node{
	struct node *left;
	struct node *right;
	struct node *parent;
	uint8_t digits[10];
	uint8_t count;
	uint64_t first;
} node;

uint64_t cube(uint64_t n){
	return n*n*n;
}

node* first_preorder(node *root){
	if(!root){
		return NULL;
	}
	if(root->left){
		return first_preorder(root->left);
	}
	if(root->right){
		return first_preorder(root->right);
	}
	return root;
}

void delete_tree(node *root){
	if(!root){
		return;
	}
	node *current = first_preorder(root);
	node *parent = current->parent;
	while(parent){
		if(current == parent->left){
			free(parent->left);
			parent->left = NULL;
		}else{
			free(parent->right);
			parent->right = NULL;
		}
		current = first_preorder(parent);
		parent = current->parent;
	}
}

node* add(node *root, uint8_t digits[10], uint64_t n){
	if(!root){
		return 0;
	}
	int o = memcmp(root->digits, digits, 10*sizeof(uint8_t));
	if(o < 0){
		if(root->left){
			return add(root->left, digits, n);
		}
		root->left = malloc(1*sizeof(node));
		if(!root->left){
			return 0;
		}
		*(root->left) = (node){.parent = root, .first = n, .count = 1};
		memcpy(root->left->digits, digits, 10*sizeof(uint8_t));
		return root->left;
	}
	if(o > 0){
		if(root->right){
			return add(root->right, digits, n);
		}
		root->right = malloc(1*sizeof(node));
		if(!root->right){
			return 0;
		}
		*(root->right) = (node){.parent = root, .first = n, .count = 1};
		memcpy(root->right->digits, digits, 10*sizeof(uint8_t));
		return root->right;
	}
	++root->count;
	return root;
}

node* add_cube(node *root, uint64_t n){
	uint64_t c = cube(n);
	uint8_t digits[10] = {0};
	while(c){
		++digits[c%10];
		c /= 10;
	}
	return add(root, digits, n);
}

int main(){
	node root = {0};
	node *c;
	for(uint64_t i = 346; i != 0; ++i){
		c = add_cube(&root, i);
		if(c->count == 5){
			printf("Found: %" PRIu64 "\n", cube(c->first));
			break;
		}
	}
	delete_tree(&root);
}

