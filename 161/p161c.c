#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define WIDTH 3
#define HEIGHT 8

typedef unsigned __int128 uint128_t;

typedef struct{
	uint128_t topHash, bottomHash;
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
                     uint128_t *blocks){//produces two top halves and ignores the center
	uint128_t c = 0;
	if(x == width){
		if(!(top%3 == 0 && bottom%3 == 0)){
			return 0;
		}
		lines->lines[lines->len++] = (Line){topHash/9, bottomHash/9};
		if(symmetric){
			return 1;
		}
		lines->lines[lines->len++] = (Line){bottomHash/9, topHash/9};
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

uint128_t *buildBlocks(size_t width, size_t *len){
	*len = 1;
	for(size_t i = 0; ++i < width; *len *= 3);
	uint128_t *ret = malloc(*len*sizeof(uint128_t));
	char counter[width];
	char block[width*2];
	memset(counter, 0, sizeof(counter));
	memset(block, 0, sizeof(block));
	size_t count = 0;
	ret[0] = countGrids(width, 2, block, 0, 0);
	for(size_t i = 1; i < *len;){
		for(size_t j = 0;;){
			++count;
			if(++counter[j] == 3){
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
			block[j + width*1] = counter[j] > 0;
			block[j + width*0] = counter[j] > 1;
			ret[i] = countGrids(width, 2, block, 0, 0);
		}
		++i;
	}
	return ret;
}

uint128_t *buildTable(size_t width, size_t len){
	uint128_t *ret = calloc(len*len, sizeof(uint128_t));
	char counter_i[width], counter_j[width], block[width*2];
	memset(counter_i, 0, sizeof(counter_i));
	size_t count_i = 0, count_j = 0;
	for(size_t i = 0, j; i < len;){
		if(count_i%3 == 0){
			memset(counter_j, 0, sizeof(counter_j));
			count_j = 0;
			while(1){
				if(count_j%3 == 0){
					j = 0;
					for(size_t k = width; k-- > 0;){
						j = j*3 + counter_j[k];
					}
					j /= 3;
					for(size_t k = 0; k < width; ++k){
						block[k + width*0] = counter_i[k] > 0 || counter_j[k] > 1;
						block[k + width*1] = counter_i[k] > 1 || counter_j[k] > 0;
						printf("%i%i %i%i\n", (int)counter_i[k], (int)counter_j[k], (int)block[k + width*0], (int)block[k + width*1]);
					}
					ret[i + width*j] = countGrids(width, 2, block, 0, 0);
					printf("(%lu, %lu) = ", i, j);print128(ret[i + width*j]);puts("");
				}
				for(size_t k = 0;;){
					++count_j;
					if(++counter_j[k] + counter_i[k] == 3){
						count_j -= counter_j[k];
						counter_j[k++] = 0;
						if(k == width){
							goto BREAK_J;
						}
					}else{
						break;
					}
				}
			}
			BREAK_J:;
			++i;
		}
		for(size_t k = 0;;){
			++count_i;
			if(++counter_i[k] == 3){
				count_i -= 3;
				counter_i[k++] = 0;
				if(k == width){
					return ret;
				}
			}else{
				break;
			}
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
	uint128_t *blocks = buildBlocks(WIDTH, &blocks_c);
	uint128_t *crosses = buildTable(WIDTH, blocks_c);
	countLines(WIDTH, 0, 0, 0, 0, 0, 1, &lines, blocks);
	lines.lines = realloc(lines.lines, lines.len*sizeof(Line));
	uint128_t c = 0, d;
	for(size_t i = 0; i < lines.len; ++i){
		for(size_t j = 0; j < lines.len; ++j){
			d = crosses[lines.lines[i].bottomHash + WIDTH*lines.lines[j].topHash]*blocks[lines.lines[i].topHash]*blocks[lines.lines[j].bottomHash];
			print128(d);puts("");
			c += d;
		}
	}
	print128(c);
	puts("");
	free(crosses);
	free(lines.lines);
	free(blocks);
}

