#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#define WIDTH 6
#define HEIGHT 12

typedef unsigned __int128 uint128_t;

typedef struct{
	char block[WIDTH*2];
	uint128_t ways;
} HalfLine;

typedef struct{
	size_t top, bottom;
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
                     Line **lines,
                     HalfLine *blocks){//produces two top halves and ignores the center
	uint128_t c = 0;
	if(x == width){
		if(!(top%3 == 0 && bottom%3 == 0))return 0;
		*((*lines)++) = (Line){topHash/9, bottomHash/9};
		if(symmetric)return 1;
		*((*lines)++) = (Line){bottomHash/9, topHash/9};
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
	char grid[width*4];
	memset(block, 0, sizeof(block));
	memset(grid, 0, sizeof(grid));
	size_t count = 0;
	for(size_t i = 0; i < *len;){
		if(count%3 == 0){
			for(size_t j = 0; j < width; ++j){
				if(block[width - 1 - j] > 0){
					ret[i].block[j + width*1] = 1;
					if(block[width - 1 - j] > 1){
						ret[i].block[j + width*0] = 1;
					}
				}
			}
			memcpy(grid + width*2, ret[i].block, width*2);
			ret[i].ways = countGrids(width, 4, grid, 0, 0);
			++i;
		}
		for(size_t j = 0;;){
			++count;
			if(++block[j] == 3){
				count -= 3;
				block[j++] = 0;
				if(j == width)return ret;
			}else break;
		}
	}
	return ret;
}

uint128_t *buildTable(size_t width, HalfLine *blocks, size_t len){
	uint128_t *ret = calloc(len*len, sizeof(uint128_t));
	for(size_t i = 0; i < len; ++i){
		char block[width*2];
		for(size_t j = 0; j < len; ++j){
			memcpy(block, blocks[i].block + width, width);
			memcpy(block + width, blocks[i].block, width);
			for(size_t k = 0; k < WIDTH*2; ++k){
				if(blocks[j].block[k]){
					if(block[k]){
						goto CONTINUE_LINE;
					}
					block[k] = 1;
				}
			}
			ret[i + len*j] = countGrids(width, 2, block, 0, 0);
			CONTINUE_LINE:;
		}
	}
	return ret;
}

int main(void){
	Line *lines = malloc(12*12*12*12*12*12*sizeof(Line)), *lines_end = lines;
	size_t lines_c, blocks_c;
	HalfLine *blocks = buildBlocks(WIDTH, &blocks_c);
	lines_c = countLines(WIDTH, 0, 0, 0, 0, 0, 1, &lines_end, blocks);
	lines = realloc(lines, lines_c*sizeof(Line));
	uint128_t *table = buildTable(WIDTH, blocks, blocks_c);
	uint128_t c = 0;
	clock_t start = clock();
	for(size_t i = 0; i < lines_c; ++i){
		//if((i + 1)%100 == 0)printf("%10zu/%10zu\n", i + 1, lines_c);
		for(size_t j = 0; j < lines_c; ++j){
			c += table[lines[i].bottom + lines[j].top*blocks_c]*blocks[lines[i].top].ways*blocks[lines[j].bottom].ways;
		}
	}
	printf("%fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
	print128(c);
	puts("");
	free(table);
	free(lines);
	free(blocks);
}

