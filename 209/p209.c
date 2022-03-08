#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static const uint64_t arity = 4;

uint64_t count_tables(uint64_t arity){
	if(arity >= 6){
		printf("Woah there, there's no way I could possibly make a table that big\n");
		exit(1);
	}
	uint64_t input_ub = 1ull << arity;
	uint64_t table_ub = 1ull << input_ub;
	uint64_t count = 0;
	for(uint64_t table = 0; table < table_ub; table += 2){
		int valid = 1;
		for(uint64_t input = 1; input < input_ub; ++input){
			if(table & (1ull << input)){
				uint64_t other = input >> 1;
				if(((input & 6) == 6) != (input & 1)){
					other |= input_ub >> 1;
				}
				if(table & (1ull << other)){
					valid = 0;
					break;
				}
			}
		}
		if(valid){
			++count;
			printf("I(");
			for(uint64_t input = 1; input < input_ub; ++input){
				if(table & (1ull << input)){
					printf("%"PRIu64",", input);
				}
			}
			printf(")\n");
		}
	}
	return count;
}

int main(){
	printf("There are %"PRIu64" valid truth tables with %"PRIu64" inputs\n", count_tables(arity), arity);
}

