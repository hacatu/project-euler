#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 7

struct edge{
	int start, end, length;
};

struct tree{
	struct tree *left, *right, *parent;
	struct edge edge;
};

struct edgeList{
	int length;
	struct tree *edges[N];
};

struct tree *tree(struct edge edge, struct tree *parent){
	struct tree *t = malloc(1*sizeof(struct tree));
	if(!t){
		return NULL;
	}
	*t = (struct tree){.left = NULL, .right = NULL, .parent = parent, .edge = edge};
	return t;
}

struct tree *add(struct tree *t, struct edge edge){
	if(edge.length == t->edge.length){
		return NULL;
	}
	if(edge.length < t->edge.length){
		if(t->left){
			return add(t->left, edge);
		}
		t->left = tree(edge, t);
		return t->left;
	}
	if(t->right){
		return add(t->right, edge);
	}
	t->right = tree(edge, t);
	return t->right;
}

struct tree *addWrapped(struct tree **t, struct edge edge){
	if(!*t){
		return *t = tree(edge, NULL);
	}
	return add(*t, edge);
}

struct tree *min(struct tree *t){
	while(t->left){
		t = t->left;
	}
	return t;
}

struct tree *max(struct tree *t){
	while(t->right){
		t = t->right;
	}
	return t;
}

void replace(struct tree *dest, struct tree *src){
	dest->edge = src->edge;
}

int isLeaf(struct tree *t){
	if(!t){
		return 0;
	}
	if(t->left){
		return 0;
	}
	if(t->right){
		return 0;
	}
	return 1;
}

void removeRoot(struct tree *t){
	struct tree *r;
	if(!t){//the node is already removed
		return;
	}
	if(isLeaf(t)){
		r = t->parent;
		if(!r){
			return free(t);
		}
		if(t == r->left){
			r->left = NULL;
		}else{
			r->right = NULL;
		}
		return free(t);
	}
	r = t->left ? max(t->left) : min(t->right);
	replace(t, r);
	return removeRoot(r);
}

void delete(struct tree *t){
	if(!t){
		return;
	}
	delete(t->left);
	delete(t->right);
	free(t);
}

int edgeLengths[N][N] = {
{INT_MAX,16,12,21,INT_MAX,INT_MAX,INT_MAX},
{16,INT_MAX,INT_MAX,17,20,INT_MAX,INT_MAX},
{12,INT_MAX,INT_MAX,28,INT_MAX,31,INT_MAX},
{21,17,28,INT_MAX,18,19,23},
{INT_MAX,20,INT_MAX,18,INT_MAX,INT_MAX,11},
{INT_MAX,INT_MAX,31,19,INT_MAX,INT_MAX,27},
{INT_MAX,INT_MAX,INT_MAX,23,11,27,INT_MAX}
};
int nodeSet[N] = {0};
struct edgeList endEdges[N] = {{0}};
struct tree *edges = NULL;

void removeEdges(int node){
	for(int i = 0; i < endEdges[node].length; ++i){
		removeRoot(endEdges[node].edges[i]);
		endEdges[node].edges[i] = 0;
	}
	endEdges[node].length = 0;
}

void addNode(int node){
	struct edge edge;
	for(int other = 0, i; other < N; ++other){
		if(nodeSet[other]){
			continue;
		}
		if(!edgeLengths[node][other]){
			continue;
		}
		i = edgeLengths[node][other];
		edge = (struct edge){.start = node, .end = other, .length = i};
		i = endEdges[other].length++;
		endEdges[other].edges[i] = addWrapped(&edges, edge);
	}
}

int main(void){
	int n = 0, sum = 0, node;
	struct edge edge;
	for(int node = 1; node < N; ++node){
		for(int other = 0; other < node; ++other){
			if(edgeLengths[node][other] != INT_MAX){
				sum += edgeLengths[node][other];
			}
		}
	}
	node = 0;
	printf("node %i\n", node);
	nodeSet[node] = 1;
	++n;
	addNode(node);
	while(1){
		edge = min(edges)->edge;
		sum -= edge.length;
		node = edge.end;
		printf("node %i\n", node);
		nodeSet[node] = 1;
		++n;
		if(n == N){
			break;
		}
		removeEdges(node);
		addNode(node);
	}
	delete(edges);
	printf("%i\n", sum);
}

