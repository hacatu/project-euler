#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "prime sieves/sieve210f.h"

#define MAX 50000000

int main(){
	FILE *file = fopen("prime sieves/primes10000.data", "r");
	if(!file){
		return 1;
	}
	uint64_t *P = readPrimeFile(file);
	fclose(file);
	if(!P){
		return 1;
	}
	int *squares = calloc(P[0], sizeof(int));
	if(!squares){
		free(P);
		return 1;
	}
	int *cubes = calloc(P[0], sizeof(int));
	if(!cubes){
		free(P);
		free(squares);
		return 1;
	}
	int *quarts = calloc(P[0], sizeof(int));
	if(!quarts){
		free(P);
		free(squares);
		free(cubes);
		return 1;
	}
	int n;
	for(size_t i = 0;; ++i){
		n = P[i + 1];
		n *= n;
		n *= n;
		if(n >= MAX){
			break;
		}
		quarts[i] = n;
	}
	for(size_t i = 0;; ++i){
		n = P[i + 1];
		n *= n*n;
		if(n >= MAX){
			break;
		}
		cubes[i] = n;
	}
	for(size_t i = 0;; ++i){
		n = P[i + 1];
		n *= n;
		if(n >=MAX){
			break;
		}
		squares[i] = n;
	}
	free(P);
	char *ns = calloc(MAX, sizeof(char));
	if(!ns){
		free(squares);
		free(cubes);
		free(quarts);
		return 1;
	}
	n = 0;
	for(size_t i = 0; squares[i]; ++i){
		for(size_t j = 0; squares[i] + cubes[j] < MAX && cubes[j]; ++j){
			for(size_t k = 0; squares[i] + cubes[j] + quarts[k] < MAX && quarts[k]; ++k){
				if(ns[squares[i] + cubes[j] + quarts[k]]){
					continue;
				}
				ns[squares[i] + cubes[j] + quarts[k]] = 1;
				++n;
			}
		}
	}
	printf("%i\n", n);
	free(squares);
	free(cubes);
	free(quarts);
	free(ns);
}

