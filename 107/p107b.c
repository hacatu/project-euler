#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <limits.h>

#define N 40

struct edge{
	int start, end, length;
};

int compareEdges(struct edge *a, struct edge *b){
	if(a->length < b->length){
		return -1;
	}
	if(a->length > b->length){
		return 1;
	}
	return 0;
}

int edgeLengths[N][N];
int nodeSet[N] = {0};
struct edge *endEdges[N][N] = {{0}};
void *edges = NULL;

