#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

#define WIDTH 6
#define HEIGHT 8

typedef unsigned __int128 uint128_t;

struct job_s;

typedef struct job_s job_t;

struct job_s{
	size_t width, height, pieces, hintX, hintY;
	int *grid;
	job_t *next;
};

typedef struct{
	job_t *jobs;
	size_t workers;
	pthread_mutex_t *lock;
} job_l_t;

void print128(uint128_t n){
	if(!n){
		printf("0");
		return;
	}
	char _out[20] = {[19] = '\0'}, *out = _out + 19;
	while(n){
		*--out = '0' + n%10;
		n /= 10;
	}
	printf(out);
}

uint128_t countGrids(size_t width, size_t height, int *grid, size_t pieces, size_t hintX, size_t hintY){
	//printf("countGrids(%lu, %lu, grid, %lu, %lu, %lu)\n", width, height, pieces, hintX, hintY);
	if(width*height == 3*pieces){
		return 1;
	}
	uint128_t c = 0;
	while(1){
		if(hintX >= width){
			hintX = 0;
			++hintY;
			continue;
		}
		if(hintY >= height){
			return 0;//can't happen
		}
		if(grid[hintX + width*hintY]){
			++hintX;
			continue;
		}
		break;
	}
	/*
	printf("(%lu, %lu)\n", hintX, hintY);
	for(size_t y = 0; y < height; ++y){
		for(size_t x = 0; x < width; ++x){
			printf(grid[x + width*y] ? "█" : " ");
		}
		puts("");
	}
	*/
	grid[hintX + width*hintY] = 1;
	if(hintY + 1 < height && !grid[hintX + width*(hintY + 1)]){//TODO: remove checks on spaces that can't be filled
		grid[hintX + width*(hintY + 1)] = 1;
		if(hintX + 1 < width){
			if(!grid[hintX + 1 + width*hintY]){//▛
				//puts("Placing ▛");
				grid[hintX + 1 + width*hintY] = 1;
				c += countGrids(width, height, grid, pieces + 1, hintX + 2, hintY);
				grid[hintX + 1 + width*hintY] = 0;
			}
			if(!grid[hintX + 1 + width*(hintY + 1)]){//▙
				//puts("Placing ▙");
				grid[hintX + 1 + width*(hintY + 1)] = 1;
				c += countGrids(width, height, grid, pieces + 1, hintX + 1, hintY);//TODO: add code to immediately add next piece
				grid[hintX + 1 + width*(hintY + 1)] = 0;
			}
		}
		if(hintX > 0 && !grid[hintX - 1 + width*(hintY + 1)]){//▟
			//puts("Placing ▟");
			grid[hintX - 1 + width*(hintY + 1)] = 1;
			c += countGrids(width, height, grid, pieces + 1, hintX + 1, hintY);
			grid[hintX - 1 + width*(hintY + 1)] = 0;
		}
		if(hintY + 2 < height && !grid[hintX + width*(hintY + 2)]){//▍
			//puts("Placing ▍");
			grid[hintX + width*(hintY + 2)] = 1;
			c += countGrids(width, height, grid, pieces + 1, hintX + 1, hintY);
			grid[hintX + width*(hintY + 2)] = 0;
		}
		grid[hintX + width*(hintY + 1)] = 0;
	}
	if(hintX + 1 < width && !grid[hintX + 1 + width*hintY]){
		grid[hintX + 1 + width*hintY] = 1;
		if(hintX + 2 < width && !grid[hintX + 2 + width*hintY]){//▂
			//puts("Placing ▂");
			grid[hintX + 2 + width*hintY] = 1;
			c += countGrids(width, height, grid, pieces + 1, hintX + 3, hintY);
			grid[hintX + 2 + width*hintY] = 0;
		}
		if(hintY + 1 < height && !grid[hintX + 1 + width*(hintY + 1)]){//▜
			//puts("Placing ▜");
			grid[hintX + 1 + width*(hintY + 1)] = 1;
			c += countGrids(width, height, grid, pieces + 1, hintX + 2, hintY);
			grid[hintX + 1 + width*(hintY + 1)] = 0;
		}
		grid[hintX + 1 + width*hintY] = 0;
	}
	grid[hintX + width*hintY] = 0;
	return c;
}

void worker(job_l_t *data){
	while(1){
		pthread_mutex_lock(data->lock);
		if(!data->jobs){
			pthread_mutex_unlock(data->lock);
			break;
		}
		job_t *job = data->jobs;
		data->jobs = data->jobs->next;
		pthread_mutex_unlock(data->lock);
		countGrids(job->width, job->height, job->grid, job->pieces, job->hintX, job->hintY);
	}
	if(!atomic_fetch_sub_explicit(&data->workers, 1, memory_order_relaxed)){
		exit(0);
	}
}

void schedule(job_l_t *data){
	pthread_t _;
	if(!data->jobs){
		return;
	}
	pthread_create(&_, NULL, (void*(*)(void*))worker, data);
	atomic_fetch_add_explicit(&data->workers, 1, memory_order_relaxed);
}

int main(void){
	int grid[WIDTH*HEIGHT] = {0};
	pthread_mutex_t lock;
	job_l_t data = {.lock = &lock};
	pthread_mutex_init(data.lock, NULL);
	//create jobs
	/*
	for(size_t cpu = sysconf(_SC_NPROCESSORS_ONLN); cpu; --cpu){
		schedule(&data);
	}
	*/
	
	uint128_t c = countGrids(WIDTH, HEIGHT, grid, 0, 0, 0);
	print128(c);
	
}

