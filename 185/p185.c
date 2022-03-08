#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "../util/factorization.h"

typedef struct{
	const char *seq;
	uint64_t matches;
} guess_t;

#define NUM_GUESSES 22
#define SEQ_LEN 16

static const guess_t guesses[NUM_GUESSES] = {
	{"5616185650518293", 2},
	{"3847439647293047", 1},
	{"5855462940810587", 3},
	{"9742855507068353", 3},
	{"4296849643607543", 3},
	{"3174248439465858", 1},
	{"4513559094146117", 2},
	{"7890971548908067", 3},
	{"8157356344118483", 1},
	{"2615250744386899", 2},
	{"8690095851526254", 3},
	{"6375711915077050", 1},
	{"6913859173121360", 1},
	{"6442889055042768", 2},
	{"2321386104303845", 0},
	{"2326509471271448", 2},
	{"5251583379644322", 2},
	{"1748270476758276", 3},
	{"4895722652190306", 1},
	{"3041631117224635", 3},
	{"1841236454324589", 3},
	{"2659862637316867", 2}
};

static inline uint64_t count_matches(const char seq[restrict static SEQ_LEN], const char guess[restrict static SEQ_LEN]){
	uint64_t res = 0;
	for(uint64_t i = 0; i < SEQ_LEN; ++i){
		res += (seq[i] == guess[i]);
	}
	return res;
}

static inline uint64_t distance(const char seq[restrict static SEQ_LEN]){
	uint64_t res = 0;
	for(uint64_t i = 0; i < NUM_GUESSES; ++i){
		uint64_t o_matches = count_matches(seq, guesses[i].seq);
		res += o_matches <= guesses[i].matches ?
			guesses[i].matches - o_matches :
			o_matches - guesses[i].matches;
	}
	return res;
}

static inline uint64_t get_closest_adj(char seq[restrict static SEQ_LEN]){
	char res[SEQ_LEN];
	uint64_t min_dist = UINT64_MAX;
	for(uint64_t i = 0; i < SEQ_LEN; ++i){
		char original = seq[i];
		for(seq[i] = '0'; seq[i] <= '9'; seq[i]++){
			if(seq[i] == original){
				continue;
			}
			uint64_t dist = distance(seq);
			if(dist < min_dist){
				memcpy(res, seq, SEQ_LEN);
				min_dist = dist;
			}
		}
		seq[i] = original;
	}
	memcpy(seq, res, SEQ_LEN);
	return min_dist;
}

static inline uint64_t get_closest_within32(char seq[restrict static SEQ_LEN]){
	uint64_t dist;
	for(uint64_t i = 0; i < 32 && (dist = get_closest_adj(seq)); ++i);
	return dist;
}

int main(){
	#pragma omp parallel
	while(1){
		char seq[SEQ_LEN + 1];
		uint64_t num = rand_u64(0, 10000000000000000);
		for(uint64_t i = 0; i < 16; ++i){
			seq[i] = '0' + num%10;
			num /= 10;
		}
		seq[SEQ_LEN] = '\0';
		if(!get_closest_within32(seq)){
			printf("%s\n", seq);
			exit(0);
		}
	}
}

