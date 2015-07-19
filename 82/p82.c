#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 80

typedef struct{
	char visited;
	unsigned long long weight;
	unsigned long long distance;
} gnode;

size_t nodec = WIDTH*HEIGHT;

gnode graph[WIDTH*HEIGHT];

size_t unvisited[WIDTH*HEIGHT];

size_t current;

int cmp_gnode_i(size_t *a, size_t *b){
	if(graph[*a].visited != graph[*b].visited){
		return graph[*a].visited - graph[*b].visited;
	}
	if(graph[*a].distance > graph[*b].distance){
		return 1;
	}
	if(graph[*a].distance < graph[*b].distance){
		return -1;
	}
	return 0;
}

void sort_unvisited(){
	qsort(unvisited, nodec, sizeof(size_t), (__compar_fn_t)cmp_gnode_i);
	current = unvisited[0];
}

void init_unvisited(){
	for(size_t n = 0; n < nodec; ++n){
		unvisited[n] = n;
	}
	sort_unvisited();
}

void calculateTop(){
	if(current < WIDTH){
		return;
	}
	unsigned long long distance = graph[current].distance + graph[current - WIDTH].weight;
	if(distance < graph[current - WIDTH].distance){
		graph[current - WIDTH].distance = distance;
	}
}

void calculateBottom(){
	if(current > WIDTH*(HEIGHT - 1) - 1){
		return;
	}
	unsigned long long distance = graph[current].distance + graph[current + WIDTH].weight;
	if(distance < graph[current + WIDTH].distance){
		graph[current + WIDTH].distance = distance;
	}
}

void calculateRight(){
	if(current%WIDTH == WIDTH - 1){
		return;
	}
	unsigned long long distance = graph[current].distance + graph[current + 1].weight;
	if(distance < graph[current + 1].distance){
		graph[current + 1].distance = distance;
	}
}

void calculateDistances(){
	calculateTop();
	calculateBottom();
	calculateRight();
}

void djkCycle(){
	calculateDistances();
	graph[current].visited = 1;
	sort_unvisited();
	--nodec;
}

int main(){
	FILE *file = fopen("p82.txt", "r");
	for(size_t i = 0; i < WIDTH*HEIGHT; ++i){
		graph[i] = (gnode){.visited = 0, .weight = -1, .distance = -1};
		fscanf(file, "%llu ", &graph[i].weight);
		if(i%WIDTH == 0){
			graph[i].distance = graph[i].weight;
		}
	}
	init_unvisited();
	while(nodec){
		djkCycle();
	}
	unsigned long long min = -1;
	for(size_t n = WIDTH - 1; n < WIDTH*HEIGHT; n += WIDTH){
		if(graph[n].distance < min){
			min = graph[n].distance;
		}
	}
	printf("%llu\n", min);
	fclose(file);
}

