#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define N 6
#define MAX_SUM 117
#define MAX_SET_STRING 111720222324
#define MIN(x, y) ((x) > (y) ? y : x)
#define INIT(i) s[i] = s[i - 1] + 1
#define UPPER(i) MIN(uR(i), sum(N - i))
#define STEP(i) ++s[i]
#define CHECK(i) if(!addSums(i)){resetSums(i);continue;}
#define LOOP_L(i) for(INIT(i); s[i] <= uR(i); STEP(i)){CHECK(i)
#define LOOP_H(i) for(INIT(i); s[i] <= UPPER(i); STEP(i)){CHECK(i)
#define END_LOOP(i) clearSums(i);}

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

uint64_t uR(uint64_t i){//mean Remainder
	//sum(i) + (N - i - 1)*s[i] + (N - i - 1)*(N - i - 1 + 1)/2 <= MAX_SUM
	//(N - i - 1)*s[i] <= MAX_SUM - sum(i) - (N - i - 1)*(N - i)/2
	//s[i] <= (MAX_SUM - sum(i))/(N - i - 1) - (N - i)/2
	return (MAX_SUM - sum(i - 1))/(N - i - 1) - (N - i)/2;
}

uint64_t addSums(uint64_t c){
	//1<<i sums will exist already
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
		//do not bother resetting subSums[i + 1<<c]
	}
}

void clearSums(uint64_t c){
	for(uint64_t i = 0; i < 1<<c; ++i){
		sums[subSums[i] + s[c]] = 0;
		//do not bother setting subSums[i + 1<<c] back to 0
	}
}

uint64_t setString(){
	uint64_t ss = 0;
	for(uint64_t i = 0; i < N; ++i){
		ss *= 100;//note: this assumes every number is two digits.
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
	LOOP_L(1)
		puts("1");
		LOOP_L(2)
			LOOP_H(3)
				LOOP_H(4)
					LOOP_H(5)
						if(sum(5) < min){
							min = sum(5);
							minSetString = setString();
						}
					END_LOOP(5)
				END_LOOP(4)
			END_LOOP(3)
		END_LOOP(2)
	END_LOOP(1)
	printf("%"PRIu64"\n", minSetString);
}

