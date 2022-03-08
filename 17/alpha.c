#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define TENS_SEP " "
#define HUNDREDS_SEP " "

#define MAX 1000000

static int cmp_strs(const void *_a, const void *_b){
	return strcmp(*(const char* const*)_a, *(const char* const*)_b);
}

inline char *stracpy(char *dest, const char *src){
	for(; *src; *dest++ = *src++);
	return dest;
}

inline static char *int2s(char *buf, uint16_t i){
	static const char *const teens[20] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
	static const char *const nties[10] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
	if(i < 20){
		return stracpy(buf, teens[i]);
	}else if(i < 100){
		buf = stracpy(buf, nties[i/10]);
		return i%10 ? stracpy(stracpy(buf, TENS_SEP), teens[i%10]) : buf;
	}
	buf = stracpy(stracpy(buf, teens[i/100]), " hundred");
	return i%100 ? int2s(stracpy(buf, HUNDREDS_SEP), i%100) : buf;
}

char *i2s(char *buf, uint64_t i){
	static const char *const lions[7] = {" ", " thousand ", " million ", " billion ", " trillion ", " quadrillion ", " quintillion "};
	if(!i){
		return stracpy(buf, "zero");
	}
	uint16_t nlions[16] = {};
	size_t j = 0;
	for(; i; nlions[j++] = i%1000, i /= 1000);
	while(j-- > 0){
		if(nlions[j]){
			buf = stracpy(int2s(buf, nlions[j]), lions[j]);
		}
	}
	*--buf = '\0';
	return buf;
}

int main(void){
	//the longest number name under one million is seven hundred seventy seven thousand seven hundred seventy seven, which is 65 bytes including null terminator.
	char **nums = malloc(MAX*sizeof(char*));
	if(!nums){
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	char *bufs = malloc(65*MAX);
	if(!bufs){
		free(nums);
		fprintf(stderr, "\e[1;31mERROR: Could not allocate memory.\e[0m\n");
		exit(EXIT_FAILURE);
	}
	for(uint64_t i = 1; i <= MAX; ++i){
		i2s((nums[i - 1] = bufs + (i - 1)*65), i);
	}
	qsort(nums, MAX, sizeof(char*), cmp_strs);
	for(size_t i = 0; i < MAX; ++i){
		puts(nums[i]);
	}
	free(nums);
	free(bufs);
}

