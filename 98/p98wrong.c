#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct{char set[26]; uint64_t digitsNumber;} Word;

//1786 words, 14 letters longest
char isSquare(uint64_t n){
	int r = sqrt(n);
	return r*r == n;
}

uint64_t digitsNumber(const char *w){
	uint64_t n = 0;
	while(*w){
		n *= 10;
		n += *w - 'A' + 1;
		++w;
	}
	return n;
}

void makeSet(const char *w, char *s){
	while(*w){
		++s[*w - 'A'];
		++w;
	}
}

int compareWords(const Word *a, const Word *b){
	int d = memcmp(a->set, b->set, 26*sizeof(char));
	if(d){
		return d;
	}
	return b->digitsNumber - a->digitsNumber;
}

int main(void){
	FILE *file = fopen("words.txt", "r");
	if(!file){
		puts("could not open file");
		return 1;
	}
	Word words[1786];
	char set[26];
	int j = 0;
	for(int i = 0; i < 1786; ++i){
		fscanf(file, "%26s", words[j].set);
		words[j].digitsNumber = digitsNumber(words[j].set);
		if(isSquare(words[j].digitsNumber)){
			strcpy(set, words[j].set);
			makeSet(set, words[j].set);
			++j;
		}
	}
	qsort(words, j, sizeof(Word), (__compar_fn_t)compareWords);
	int max = 0;
	for(int i = 0; i < j; ++i){
		if(memcmp(words[i].set, words[i + 1].set, 26*sizeof(char))){
			continue;
		}
		if(words[i].digitsNumber > max){
			max = words[i].digitsNumber;
		}
		do{
			++i;
		}while(!memcmp(words[i].set, words[i + 1].set, 26*sizeof(char)));
	}
	printf("%i\n", j);
}

