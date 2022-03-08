#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

uint64_t farey(uint64_t n){
	uint64_t ret = 1;
	for(uint64_t a = 0, b = 1, c = 1, d = n; c != d;){
		if(c + d <= n + 1){
			++ret;
		}
		uint64_t p = (n + b)/d, q;
		q = p*d - b;
		p = p*c - a;
		a = c;
		b = d;
		c = p;
		d = q;
	}
	return ret;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Invalid invocation, please use like:\n"
			"%s <hexagon size>\n", argv[0]);
		exit(1);
	}
	uint64_t n;
	sscanf(argv[1], "%"SCNu64, &n);
	printf("%"PRIu64": %"PRIu64"\n", n, 3*(n + 1)*n - 12*farey(n - 1));
}

