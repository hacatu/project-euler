#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define WIDTH 3
#define HEIGHT 8

typedef unsigned __int128 uint128_t;

typedef struct{
	char block[WIDTH*2];
	uint128_t ways;
} HalfLine;

typedef struct{
	HalfLine *top, *bottom;
} Line;

typedef struct{
	Line *lines;
	size_t len;
} Lines;

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

uint128_t countGrids(size_t width, size_t height, char *grid, size_t x, size_t y){
	uint128_t c = 0;
	do{
		if(x >= width){
			x = 0;
			++y;
		}
		if(y >= height)return 1;
	}while(grid[x + width*y] && ++x);
	while(grid[x + width*y] ^= 1){//on and then off
		if(y + 1 < height && !grid[x + width*(y + 1)])
		while(grid[x + width*(y + 1)] ^= 1){
			if(x + 1 < width){
				if(!grid[x + 1 + width*y])
				while(grid[x + 1 + width*y] ^= 1)
					c += countGrids(width, height, grid, x + 2, y);//▛
				if(!grid[x + 1 + width*(y + 1)])
				while(grid[x + 1 + width*(y + 1)] ^= 1)
					c += countGrids(width, height, grid, x + 1, y);//▙
			}
			if(x > 0 && !grid[x - 1 + width*(y + 1)])
			while(grid[x - 1 + width*(y + 1)] ^= 1)
				c += countGrids(width, height, grid, x + 1, y);//▟
			if(y + 2 < height && !grid[x + width*(y + 2)])
			while(grid[x + width*(y + 2)] ^= 1)
				c += countGrids(width, height, grid, x + 1, y);//▍
		}
		if(x + 1 < width && !grid[x + 1 + width*y])
		while(grid[x + 1 + width*y] ^= 1){
			if(x + 2 < width && !grid[x + 2 + width*y])
			while(grid[x + 2 + width*y] ^= 1)
				c += countGrids(width, height, grid, x + 3, y);//▂
			if(y + 1 < height && !grid[x + 1 + width*(y + 1)])
			while(grid[x + 1 + width*(y + 1)] ^= 1)
				c += countGrids(width, height, grid, x + 2, y);//▜
		}
	}
	return c;
}

uint128_t countLines(size_t width,
                     uint128_t topHash,//the hashes encode the top and bottom block as ... 9*[x - 2] + 3*[x - 1] + 1*[x]
                     uint128_t bottomHash,//so to advance x n spaces, call with hash*3**n and x + n.  Also add 3**(n + s)*l to hash to add l blocks to the row s from x
                     size_t x,
                     size_t top,//the number of blocks on the top.  Must be a multiple of 3.  Becase of this, the hashes can be divided by 9 to produce unique codes for everything
                     size_t bottom,//these codes are the indicies for the blocks list.
                     int symmetric,
                     Lines *lines,
                     HalfLine *blocks){//produces two top halves and ignores the center
	uint128_t c = 0;
	if(x == width){
		if(!(top%3 == 0 && bottom%3 == 0)){
			return 0;
		}
		lines->lines[lines->len++] = (Line){.top = blocks + topHash/9, .bottom = blocks + bottomHash/9};
		if(symmetric){
			return 1;
		}
		lines->lines[lines->len++] = (Line){.top = blocks + bottomHash/9, .bottom = blocks + topHash/9};
		return 2;
	}
	if(symmetric){
		c += countLines(width, 3*topHash + 3*2, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▍
		c += countLines(width, 3*topHash + 3*1, 3*bottomHash + 3*1, x + 1, top + 1, bottom + 1, 1, lines, blocks);//-▍
		if(x > 0 && topHash/3%3 == 0){
			c += countLines(width, 3*topHash + 9*1 + 3*1, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▜
		}
		if(x + 1 < width){
			c += countLines(width, 3*topHash + 3*1 + 1*1, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▛
			c += countLines(width, 9*topHash + 9*1, 9*bottomHash, x + 2, top + 1, bottom, 0, lines, blocks);//_▙
		}
		if(x + 1 < width){
			if(x + 2 < width){
				c += countLines(width, 27*topHash, 27*bottomHash, x + 3, top, bottom, 1, lines, blocks);//▂
			}
			c += countLines(width, 9*topHash + 3*1, 9*bottomHash, x + 2, top + 1, bottom, 0, lines, blocks);//_▟
		}
	}else{
		if(topHash%3 == 0){
			c += countLines(width, 3*topHash + 3*2, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▍
			if(bottomHash%3 == 0){
				c += countLines(width, 3*topHash + 3*1, 3*bottomHash + 3*1, x + 1, top + 1, bottom + 1, 0, lines, blocks);//-▍
			}
			if(x + 1 < width){
				c += countLines(width, 3*topHash + 3*1 + 1*1, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▛
				c += countLines(width, 9*topHash + 9*1, 9*bottomHash, x + 2, top + 1, bottom, 0, lines, blocks);//_▙
			}
			if(x > 0 && topHash/3%3 == 0){
				c += countLines(width, 3*topHash + 9*1 + 3*1, 3*bottomHash, x + 1, top + 2, bottom, 0, lines, blocks);//_▜
			}
		}
		if(bottomHash%3 == 0){
			c += countLines(width, 3*topHash, 3*bottomHash + 3*2, x + 1, top, bottom + 2, 0, lines, blocks);//`▍
			if(x + 1 < width){
				c += countLines(width, 3*topHash, 3*bottomHash + 3*1 + 1*1, x + 1, top, bottom + 2, 0, lines, blocks);//-▙
				c += countLines(width, 9*topHash, 9*bottomHash + 9*1, x + 2, top, bottom + 1, 0, lines, blocks);//-▛
			}
			if(x > 0 && bottomHash/3%3 == 0){
				c += countLines(width, 3*topHash, 3*bottomHash + 9*1 + 3*1, x + 1, top, bottom + 2, 0, lines, blocks);//-▟
			}
		}
		if(x + 1 < width){
			c += countLines(width, 9*topHash + 3*1, 9*bottomHash, x + 2, top + 1, bottom, 0, lines, blocks);//_▟
			c += countLines(width, 9*topHash, 9*bottomHash + 3*1, x + 2, top, bottom + 1, 0, lines, blocks);//-▜
			if(x + 2 < width){
				c += countLines(width, 27*topHash, 27*bottomHash, x + 3, top, bottom, 0, lines, blocks);//▂
			}
		}
	}
	return c;
}

HalfLine *buildBlocks(size_t width, size_t *len){
	*len = 1;
	for(size_t i = 0; ++i < width; *len *= 3);
	HalfLine *ret = calloc(*len, sizeof(HalfLine));
	char block[width];
	memset(block, 0, width*sizeof(char));
	size_t count = 0;
	for(size_t j = 0; j < width; ++j){
		if(block[j] > 0){
			ret[0].block[j + width*1] = 1;
			if(block[j] > 1){
				ret[0].block[j + width*0] = 1;
			}
		}
	}
	for(size_t i = 1; i < *len;){
		for(size_t j = 0;;){
			++count;
			if(++block[j] == 3){
				count -= 3;
				block[j++] = 0;
				if(j == width){
					printf("returning early with i == %lu\n", i);
					return ret;
				}
			}else{
				break;
			}
		}
		if(count%3){
			continue;
		}
		for(size_t j = 0; j < width; ++j){
			if(block[j] > 0){
				ret[i].block[j + width*1] = 1;
				if(block[j] > 1){
					ret[i].block[j + width*0] = 1;
				}
			}
		}
		++i;
	}
	return ret;
}

void fillBlocks(size_t width, HalfLine *blocks, size_t len){
	//char grid[width*4];
	char grid[width*2];
	//memset(grid, 0, sizeof(grid));
	for(size_t i = 0, j, r; i < len; ++i){
		for(j = r = 0; j < width; ++j){
			//printf("%lu,", (size_t)blocks[i].block[j + width*1] + blocks[i].block[j + width*0]);
			r = 3*r + blocks[i].block[j + width*1] + blocks[i].block[j + width*0];
		}
		r /= 3;
		//printf(": %lu, %lu\n", i, r);
		if(r < i){
			blocks[i].ways = blocks[r].ways;
			continue;
		}
		//memcpy(grid + width*2, blocks[i].block, width*2*sizeof(char));
		memcpy(grid, blocks[i].block, width*2*sizeof(char));
		//blocks[i].ways = countGrids(width, 4, grid, 0, 0);
		blocks[i].ways = countGrids(width, 2, grid, 0, 0);
	}
}

uint128_t *buildCrosses(size_t width, size_t *len){
	//1/3rd of these will never be filled and of the remaining some will have 0 solutions but this is the best I can do while maintaining constant look up time.
	*len = (1<<width)<<(width - 1);
	uint128_t *ret = calloc(*len, sizeof(uint128_t));
	char grid[width*2];
	for(size_t i = 0, j, r, p; i < 1<<width; ++i){
		for(j = i, p = 0; p < width; ++p, j >>= 1){
			grid[p] = j&1;
		}
		if(__builtin_popcount(i)%3 == 0){//Handles horizontal symmetry
			memcpy(grid + width, grid, width*sizeof(char));
			ret[((i<<width) + i)>>1] = countGrids(width, 2, grid, 0, 0);
			printf("ret[%lu]:", ((i<<width) + i));print128(ret[((i<<width) + i)>>1]);puts("");
		}
		for(size_t j = 0; j < i; ++j){
			if((__builtin_popcount(i) + __builtin_popcount(j))%3){
				continue;
			}
			for(r = j, p = 0; p < width; ++p, r >>= 1){
				grid[p + width] = r&1;
			}
			ret[((i<<width) + j)>>1] = ret[((j<<width) + i)>>1] = countGrids(width, 2, grid, 0, 0);
			printf("ret[%lu]:", ((i<<width) + j));print128(ret[((i<<width) + j)>>1]);puts("");
			printf("ret[%lu]:", ((j<<width) + i));print128(ret[((j<<width) + i)>>1]);puts("");
		}
	}
	return ret;
}

uint128_t *buildTable(size_t width, HalfLine *blocks, size_t len, uint128_t *crosses){
	uint128_t *ret = calloc(len*len, sizeof(uint128_t));
	for(size_t i = 0; i < len; ++i){
		char block[width*2];
		for(size_t j = 0, w, r, k; j <= i; ++j){
			memcpy(block, blocks[i].block + width, width*sizeof(char));
			memcpy(block + width, blocks[i].block, width*sizeof(char));
			for(k = r = 0; k < width*2; ++k){
				if(blocks[j].block[k]){
					if(block[k]){
						goto CONTINUE_LINE;
					}
					block[k] = 1;
				}
				r = (r<<1) + block[k];
			}
			r >>= 1;
			w = crosses[r];
			ret[i + width*j] = ret[j + width*i] = w;
			CONTINUE_LINE:;
		}
	}
	return ret;
}

int main(void){
	//char grid[WIDTH*HEIGHT] = {0};
	//uint128_t c = countGrids(WIDTH, HEIGHT, grid, 0, 0);
	//char grid[WIDTH*4] = {0};
	Lines lines = {.lines = malloc(12*12*12*12*12*12*sizeof(Line))};
	size_t blocks_c, crosses_c;
	HalfLine *blocks = buildBlocks(WIDTH, &blocks_c);
	countLines(WIDTH, 0, 0, 0, 0, 0, 1, &lines, blocks);
	lines.lines = realloc(lines.lines, lines.len*sizeof(Line));
	fillBlocks(WIDTH, blocks, blocks_c);
	uint128_t *crosses = buildCrosses(WIDTH, &crosses_c);
	uint128_t *table = buildTable(WIDTH, blocks, blocks_c, crosses);
	free(crosses);
	uint128_t c = 0, a;
	
	for(size_t i = 0; i < lines.len; ++i){
		//printf("%lu/%lu\n", i, lines.len);
		char block[WIDTH*2];
		for(size_t j = 0; j < lines.len; ++j){
			memcpy(block, lines.lines[i].bottom->block + WIDTH, WIDTH*sizeof(char));
			memcpy(block + WIDTH, lines.lines[i].bottom->block, WIDTH*sizeof(char));
			for(size_t k = 0; k < WIDTH*2; ++k){
				if(lines.lines[j].top->block[k]){
					if(block[k]){
						goto CONTINUE_LINE;
					}
					block[k] = 1;
				}
			}
			a = countGrids(WIDTH, 2, block, 0, 0);
			c += a*lines.lines[i].top->ways*lines.lines[j].bottom->ways;
			CONTINUE_LINE:;
		}
	}
	
	/*
	for(size_t i = 0; i < lines.len; ++i){
		for(size_t j = 0; j < lines.len; ++j){
			c += table[(lines.lines[i].bottom - blocks) + WIDTH*(lines.lines[j].top - blocks)]*lines.lines[i].top->ways*lines.lines[j].bottom->ways;
		}
	}
	*/
	/*
	for(size_t i = 0; i < lines.len; ++i){
		c += lines.lines[i].top->ways*lines.lines[i].bottom->ways;
	}
	*/
	print128(c);
	puts("");
	free(table);
	free(lines.lines);
	free(blocks);
}

