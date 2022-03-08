#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint64_t calcVisible(uint64_t n, uint64_t visible[static n + 1]){
	if(!visible[n]){
		visible[n] = 3*n*(n + 1);
		for(uint64_t m = 2; m < n; ++m){
			visible[n] -= calcVisible(n/m, visible);
		}
	}
	return visible[n];
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Invalid invocation, please use like:\n"
			"%s <hexagon size>\n", argv[0]);
		exit(1);
	}
	uint64_t n;
	sscanf(argv[1], "%"SCNu64, &n);
	uint64_t *visible = calloc(n + 1, sizeof(uint64_t));
	printf("%"PRIu64": %"PRIu64"\n", n, 3*n*(n + 1) - calcVisible(n, visible));
	free(visible);
}

