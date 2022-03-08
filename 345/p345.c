#include <stdio.h>
#include <inttypes.h>

#define MAT_WIDTH 5
#define MAT_HEIGHT 5

typedef struct{
	int64_t val;
	size_t i;
} mat_ent;

typedef struct{
	mat_ent *ents;
	size_t j;
} mat_row;

int64_t matrix[MAT_HEIGHT][MAT_WIDTH] = {
{  7,  53, 183, 439, 863},
{497, 383, 563,  79, 973},
{287,  63, 343, 169, 583},
{627, 343, 773, 959, 943},
{767, 473, 103, 699, 303}
};

void heapify_ents(mat_ent *ents){
	
}

int64_t matrix_sum(void){
	mat_ent ent_buf[MAT_WIDTH*MAT_HEIGHT];
	mat_row rows_heap[MAT_HEIGHT];
	for(size_t j = 0; j < MAT_HEIGHT; ++j){
		rows_heap[j] = (mat_row){.ents= ent_buf + MAT_WIDTH*j, .j=j};
		for(size_t i = 0; i < MAT_WIDTH; ++i){
			rows_heap[j].ents[i] = (mat_ent){.val= matrix[j][i], .i=i};
		}
		
	}
}

int main(void){
	
}

