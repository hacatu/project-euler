#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define N 6
#define MAX_SUM 117
#define MAX_SET_STRING 111720222324
#define MIN(x, y) ((x) > (y) ? y : x)

uint64_t s[N];
char sums[MAX_SUM + 1];
uint64_t subSums[1<<N];

uint64_t sum(uint64_t i){
	uint64_t t = 0;
	for(; i; --i){
		t += s[i];
	}
	return t;
}

uint64_t uR(uint64_t i){//mean remainder
	//sum(i) + (N - i - 1)*s[i] + (N - i - 1)*(N - i - 1 + 1)/2 <= MAX_SUM
	//(N - i - 1)*s[i] <= MAX_SUM - sum(i) - (N - i - 1)*(N - i)/2
	//s[i] <= (MAX_SUM - sum(i))/(N - i - 1) - (N - i)/2
	uint64_t ret = (MAX_SUM - sum(i - 1))/(N - i) - (N - i)/2;
	if(i == 1)printf("uR  :%"PRIu64"\n", ret);
	return ret;
}

uint64_t addSums(uint64_t c){
	uint64_t t;
	for(uint64_t i = 0; i < 1<<c; ++i){
		t = subSums[i] + s[c];
		if(sums[t]){
			return 0;
		}
		sums[t] = 1;
		subSums[i + (1<<c)] = t;
	}
	return 1;
}

void resetSums(uint64_t c){
	for(uint64_t i = 0, t = subSums[i] + s[c]; sums[t]; ++i, t = subSums[i] + s[c]){
		sums[t] = 0;
	}
}

void clearSums(uint64_t c){
	for(uint64_t i = 0; i < 1<<c; ++i){
		sums[subSums[i] + s[c]] = 0;
	}
}

uint64_t setString(){
	uint64_t ss = 0;
	for(uint64_t i = 0; i < N; ++i){
		ss *= 100;
		ss += s[i];
	}
	return ss;
}

int main(void){
	s[0] = 11;
	subSums[0] = 0; subSums[1] = s[0];
	sums[subSums[0]] = 1; sums[subSums[1]] = 1;
	uint64_t min = MAX_SUM;
	uint64_t minSetString = MAX_SET_STRING;
	for(s[1] = s[1 - 1] + 1; s[1] <= uR(1); ++s[1]){
		printf("s[i]:%"PRIu64"\n", s[1]);
		if(!addSums(1)){
			resetSums(1);
			continue;
		}
		for(s[2] = s[2 - 1] + 1; s[2] <= uR(2); ++s[2]){
			if(!addSums(2)){
				resetSums(2);
				continue;
			}
			for(s[3] = s[3 - 1] + 1; s[3] <= MIN(uR(3), sum(N - 3)); ++s[3]){
				if(!addSums(3)){
					resetSums(3);
					continue;
				}
				for(s[4] = s[4 - 1] + 1; s[4] <= MIN(uR(4), sum(N - 4)); ++s[4]){
					if(!addSums(4)){
						resetSums(4);
						continue;
					}
					for(s[5] = s[5 - 1] + 1; s[5] <= MIN(uR(5), sum(N - 5)); ++s[5]){
						if(!addSums(5)){
							resetSums(5);
							continue;
						}
						if(sum(5) < min){
							min = sum(5);
							minSetString = setString();
						}
						clearSums(5);}
					clearSums(4);}
				clearSums(3);}
			clearSums(2);}
		clearSums(1);}
	printf("%"PRIu64"\n", minSetString);
}
