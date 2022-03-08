#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t arity = 6;

void print_edges(uint64_t arity){
	if(arity >= 64){
		printf("Woah there, there's no way I could possibly make a table that big\n");
		exit(1);
	}
	uint64_t input_ub = 1ull << arity;
	uint64_t *exclude1 = malloc(input_ub*sizeof(uint64_t));
	uint64_t *exclude2 = malloc(input_ub*sizeof(uint64_t));
	uint8_t *visited = calloc(input_ub, sizeof(uint8_t));
	for(uint64_t input = 1; input < input_ub; ++input){
		uint64_t other = input >> 1;
		if(((input & 6) == 6) != (input & 1)){
			other |= input_ub >> 1;
		}
		printf("%"PRIu64"->%"PRIu64"\n", input, other);
		exclude1[input] = other;
		exclude2[other] = input;
	}
	puts("----------");
	for(uint64_t input = 1; input < input_ub; ++input){
		for(uint64_t n = input; !visited[n]; n = exclude1[n]){
			visited[n] = 1;
			printf("%"PRIu64",", n);
		}
		puts("");
	}
	free(exclude1);
	free(exclude2);
	free(visited);
}

uint64_t cycle_independent_sets(uint64_t n){
	uint64_t f1 = 1, f0 = 2;
	while(n--){
		uint64_t t = f1;
		f1 += f0;
		f0 = t;
	}
	return f0;
}

int main(){
	print_edges(arity);
	printf("L(2)=%"PRIu64", L(3)=%"PRIu64", L(6)=%"PRIu64", L(46)=%"PRIu64"\n",
		cycle_independent_sets(2),
		cycle_independent_sets(3),
		cycle_independent_sets(6),
		cycle_independent_sets(46)
	);
}

