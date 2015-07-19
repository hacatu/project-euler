#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

typedef struct{char set[26], word[15];} Word;

//1786 words, 14 letters longest
char isSquare(uint64_t n){
	int r = sqrt(n);
	return r*r == n;
}

void makeSet(const char *w, char *s){
	while(*w){
		++s[*w - 'A'];
		++w;
	}
}

int compareBySet(const Word *a, const Word *b){
	return memcmp(a->set, b->set, sizeof(a->set));
}

int compareByLen(const Word *a, const Word *b){
	int d = strlen(b->word) - strlen(a->word);
	if(d){
		return d;
	}
	return compareBySet(a, b);
}

int processCombination(Word a, Word b){
	
}

int processCombinations(Word *words, int a, int b){
	int max = 0, cur;
	for(int i = a; i < b - 1; ++i){
		for(int j = i + 1; j < b; ++j){
			cur = processCombination(words[i], words[j]);
			if(cur > max){
				max = cur;
			}
		}
	}
	return max;
}

int main(void){
	FILE *file = fopen("words.txt", "r");
	if(!file){
		puts("could not open file");
		return 1;
	}
	Word words[1786];
	for(int i = 0; i < 1786; ++i){
		fscanf(file, "%26s", words[i].word);
		makeSet(words[i].word, words[i].set);
	}
	fclose(file);
	qsort(words, 1786, sizeof(Word), (__compar_fn_t)compareBySet);
	Word anagrams[1786];
	int l = 0;
	for(int i = 0; i < 1786; ++i){
		if(compareBySet(words + i, words + i + 1)){
			continue;
		}
		while(!compareBySet(words + i, words + i + 1)){
			anagrams[l++] = words[i++];
		}
		anagrams[l++] = words[i];
	}
	qsort(anagrams, l, sizeof(Word), (__compar_fn_type)compareByLen);
	int max = 0;
	for(int i = 0, j = 1; i < l; i = j++){
		while(!compareBySet(words + i, words + j)){
			++j;
		}
		if((max = processCombinations(words, i, j))){
			break;
		}
	}
	printf("%i\n", max);
}

