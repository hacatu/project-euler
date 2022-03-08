#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../util/hash.h"

//we store left, right, m_here, m_max, and of course up to 7 parts of a partition
//left ranges from 0 to 39 so it gets 6 bits, right ranges from 0 to 19 so it gets 5
//m_here and m_max range from 1 to 20 so they get 5
//size1 ranges from 1 to 38 (6 bits) so count1 ranges from 1 to 20 (5 bits)
//size2 ranges from 2 to 36 (6 bits) so count2 ranges from 1 to 12 (4 bits)
//size3 ranges from 3 to 33 (6 bits) so count3 ranges from 1 to 8  (4 bits)
//size4 ranges from 4 to 29 (5 bits) so count4 ranges from 1 to 6  (3 bits)
//size5 ranges from 5 to 24 (5 bits) so count5 ranges from 1 to 4  (3 bits)
//size6 ranges from 6 to 18 (5 bits) so count6 ranges from 1 to 2  (2 bits)
//size7 ranges from 7 to 11 (4 bits) so count7 ranges from 1 to 1  (0 bits)

typedef struct __attribute__((packed)) {
	uint64_t left:6, right:6, m_here:5, m_max:5,
		size1:6, count1:5,
		size2:6, count2:4,
		size3:6, count3:4,
		size4:5, count4:3,
		         count5:3;
	uint16_t
		size5:5,
		size6:5, count6:2,
		size7:4;
	double count;
} case_ent;

static uint64_t hash_case_ent(const void *_a){
	return *(const uint64_t*)_a ^ ((const uint16_t*)_a)[4];
}

static int cmp_case_ent(const void *_a, const void *_b){
	return memcmp(_a, _b, offsetof(case_ent, count));
}

static int add_case_ent(void *_a, void *_b){
	((case_ent*)_a)->count += ((const case_ent*)_b)->count;
	return 1;
}

static hashtbl_ft ft = {
	.size=sizeof(case_ent),
	.hash=hash_case_ent,
	.cmp=cmp_case_ent,
	.add=add_case_ent,
	.load_factor=.7
};

static inline void case_ent_get_part(const case_ent *ent, uint64_t i, uint64_t *size, uint64_t *mult){
	switch(i){
	case 0:
		*size = ent->size1, *mult = ent->count1;
		break;
	case 1:
		*size = ent->size2, *mult = ent->count2;
		break;
	case 2:
		*size = ent->size3, *mult = ent->count3;
		break;
	case 3:
		*size = ent->size4, *mult = ent->count4;
		break;
	case 4:
		*size = ent->size5, *mult = ent->count5;
		break;
	case 5:
		*size = ent->size6, *mult = ent->count6;
		break;
	case 6:
		*size = ent->size7, *mult = !!ent->size7;
		break;
	}
}

static inline uint64_t case_ent_unused_len(const case_ent *ent){
	uint64_t res = ent->left + ent->right, size, mult;
	for(uint64_t i = 0; i < 7; ++i){
		case_ent_get_part(ent, i, &size, &mult);
		if(!mult){
			break;
		}
		res += size*mult;
	}
	return res;
}

int print_case_ent(const case_ent *ent){
	int res = printf(" (left:%d, right:%d, m_here:%d, m_max:%d, partition:(", ent->left, ent->right, ent->m_here, ent->m_max);
	uint64_t size, mult;
	for(uint64_t i = 0; i < 7; ++i){
		case_ent_get_part(ent, i, &size, &mult);
		if(!mult){
			break;
		}
		if(mult == 1){
			res += printf("%"PRIu64",", size);
		}else{
			res += printf("%"PRIu64"^%"PRIu64",", size, mult);
		}
	}
	return res + printf(")): %f\n", ent->count);
}

static inline void check_case_ent(const case_ent *ent, const case_ent *out){
	if(case_ent_unused_len(out) != case_ent_unused_len(ent) - 1){
		printf("Bad partition detected ");
		print_case_ent(out);
		printf(", debug me\n");
		exit(1);
	}
}

static inline void case_ent_set_part(case_ent *ent, uint64_t i, uint64_t size, uint64_t mult){
	switch(i){
	case 0:
		ent->size1 = size, ent->count1 = mult;
		break;
	case 1:
		ent->size2 = size, ent->count2 = mult;
		break;
	case 2:
		ent->size3 = size, ent->count3 = mult;
		break;
	case 3:
		ent->size4 = size, ent->count4 = mult;
		break;
	case 4:
		ent->size5 = size, ent->count5 = mult;
		break;
	case 5:
		ent->size6 = size, ent->count6 = mult;
		break;
	case 6:
		ent->size7 = size;
		break;
	}
}

static inline uint64_t case_ent_lsearch_part(const case_ent *ent, uint64_t a, uint64_t b, uint64_t qsize){
	for(uint64_t i = a; i < b; ++i){
		uint64_t size, mult;
		case_ent_get_part(ent, i, &size, &mult);
		if(!mult || size >= qsize){
			return i;
		}
	}
	return b;
}

static inline void add_part(case_ent *out, const case_ent *ent, uint64_t size){
	uint64_t i = case_ent_lsearch_part(ent, 0, 7, size), k_in, k_out;
	uint64_t size1, mult;
	if(
		i == 7 ||
		(case_ent_get_part(ent, i, &size1, &mult), !mult) ||
		size1 != size
	){
		for(k_in = k_out = 0; k_in < i; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size1, &mult);
			case_ent_set_part(out, k_out, size1, mult);
		}
		case_ent_set_part(out, k_out++, size, 1);
		for(; k_in < 7; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size1, &mult);
			if(!mult){
				break;
			}
			case_ent_set_part(out, k_out, size1, mult);
		}
		for(; k_out < 7; ++k_out){
			case_ent_set_part(out, k_out, 0, 0);
		}
	}else{
		for(k_in = k_out = 0; k_in < i; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size1, &mult);
			case_ent_set_part(out, k_out, size1, mult);
		}
		case_ent_get_part(ent, i, &size1, &mult);
		case_ent_set_part(out, i, size, mult + 1);
		for(k_in = k_out = i + 1; k_in < 7; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size1, &mult);
			if(!mult){
				break;
			}
			case_ent_set_part(out, k_out, size1, mult);
		}
		for(; k_out < 7; ++k_out){
			case_ent_set_part(out, k_out, 0, 0);
		}
	}
}

static inline void split_part(case_ent *out, const case_ent *ent, uint64_t i, uint64_t j){
	uint64_t size, mult, size1, mult1, k_in, k_out;
	case_ent_get_part(ent, i, &size1, &mult1);
	uint64_t s = case_ent_lsearch_part(ent, 0, i, j);
	uint64_t l = case_ent_lsearch_part(ent, s, i, size1 - j - 1);
	for(k_in = k_out = 0; k_out < s; ++k_out, ++k_in){
		case_ent_get_part(ent, k_in, &size, &mult);
		case_ent_set_part(out, k_out, size, mult);
	}
	case_ent_get_part(ent, s, &size, &mult);
	if(size == j){
		case_ent_set_part(out, s, j, mult + 1);
		for(k_in = k_out = s + 1; k_in < l; ++k_out, ++k_in){
			case_ent_get_part(ent, k_in, &size, &mult);
			case_ent_set_part(out, k_out, size, mult);
		}
	}else{
		case_ent_set_part(out, s, j, 1);
		for(k_in = s, k_out = s + 1; k_in < l; ++k_out, ++k_in){
			case_ent_get_part(ent, k_in, &size, &mult);
			case_ent_set_part(out, k_out, size, mult);
		}
	}
	case_ent_get_part(ent, l, &size, &mult);
	if(size == size1 - j - 1){
		case_ent_set_part(out, k_out++, size, mult + 1);
		for(++k_in; k_in < i; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size, &mult);
			case_ent_set_part(out, k_out, size, mult);
		}
	}else{
		case_ent_set_part(out, k_out++, size1 - j - 1, 1);
		for(; k_in < i; ++k_in, ++k_out){
			case_ent_get_part(ent, k_in, &size, &mult);
			case_ent_set_part(out, k_out, size, mult);
		}
	}
	if(mult1 != 1){
		case_ent_set_part(out, k_out++, size1, mult1 - 1);
	}
	for(++k_in; k_in < 7; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		if(!mult){
			break;
		}
		case_ent_set_part(out, k_out, size, mult);
	}
	for(; k_out < 7; ++k_out){
		case_ent_set_part(out, k_out, 0, 0);
	}
}

static inline void shorten_part(case_ent *out, const case_ent *ent, uint64_t i){
	uint64_t size, mult, size1, mult1, k_in, k_out;
	case_ent_get_part(ent, i, &size1, &mult1);
	if(mult1 == 1){
		if(i && (case_ent_get_part(ent, i - 1, &size, &mult), size == size1 - 1)){
			for(k_in = k_out = 0; k_in < i - 1; ++k_in, ++k_out){
				case_ent_get_part(ent, k_in, &size, &mult);
				case_ent_set_part(out, k_out, size, mult);
			}
			case_ent_get_part(ent, k_in++, &size, &mult);
			case_ent_set_part(out, k_out++, size, mult + 1);
		}else{
			for(k_in = k_out = 0; k_in < i; ++k_in, ++k_out){
				case_ent_get_part(ent, k_in, &size, &mult);
				case_ent_set_part(out, k_out, size, mult);
			}
			case_ent_set_part(out, k_out++, size1 - 1, 1);
		}
	}else{
		if(i && (case_ent_get_part(ent, i - 1, &size, &mult), size == size1 - 1)){
			for(k_in = k_out = 0; k_in < i - 1; ++k_in, ++k_out){
				case_ent_get_part(ent, k_in, &size, &mult);
				case_ent_set_part(out, k_out, size, mult);
			}
			case_ent_get_part(ent, k_in++, &size, &mult);
			case_ent_set_part(out, k_out++, size, mult + 1);
			case_ent_set_part(out, k_out++, size1, mult1 - 1);
		}else{
			for(k_in = k_out = 0; k_in < i; ++k_in, ++k_out){
				case_ent_get_part(ent, k_in, &size, &mult);
				case_ent_set_part(out, k_out, size, mult);
			}
			case_ent_set_part(out, k_out++, size1 - 1, 1);
			case_ent_set_part(out, k_out++, size1, mult1 - 1);
		}
	}
	for(k_in = i + 1; k_in < 7; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		if(!mult){
			break;
		}
		case_ent_set_part(out, k_out, size, mult);
	}
	for(; k_out < 7; ++k_out){
		case_ent_set_part(out, k_out, 0, 0);
	}
}

static inline void remove_part(case_ent *out, const case_ent *ent, uint64_t i){
	uint64_t size, mult, k_in, k_out;
	for(uint64_t j = 0; j < i; ++j){
		case_ent_get_part(ent, j, &size, &mult);//unreachable because we only remove size 1 parts which must be first
		case_ent_set_part(out, j, size, mult);
	}
	case_ent_get_part(ent, i, &size, &mult);
	if(mult == 1){
		k_in = i + 1;
		k_out = i;
	}else{
		case_ent_set_part(out, i, size, mult - 1);
		k_in = i + 1;
		k_out = i + 1;
	}
	for(; k_in < 7; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		if(!mult){
			break;
		}
		case_ent_set_part(out, k_out, size, mult);
	}
	for(; k_out < 7; ++k_out){
		case_ent_set_part(out, k_out, 0, 0);
	}
}

static inline void bisect_part(case_ent *out, const case_ent *ent, uint64_t i, uint64_t half_size){
	uint64_t size, mult, k_in, k_out;
	uint64_t j = case_ent_lsearch_part(ent, 0, i, half_size);
	for(k_in = k_out = 0; k_in < j; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		case_ent_set_part(out, k_out, size, mult);
	}
	case_ent_get_part(ent, j, &size, &mult);
	if(size == half_size){
		case_ent_set_part(out, k_out++, half_size, mult + 2);
		k_in = j + 1;
	}else{
		case_ent_set_part(out, k_out++, half_size, 2);
	}
	for(; k_in < i; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		case_ent_set_part(out, k_out, size, mult);
	}
	case_ent_get_part(ent, i, &size, &mult);
	if(mult != 1){
		case_ent_set_part(out, k_out++, size, mult - 1);
	}
	for(k_in = i + 1; k_in < 7; ++k_in, ++k_out){
		case_ent_get_part(ent, k_in, &size, &mult);
		if(!mult){
			break;
		}
		case_ent_set_part(out, k_out, size, mult);
	}
	for(; k_out < 7; ++k_out){
		case_ent_set_part(out, k_out, 0, 0);
	}
}

void process_inner_unselected(case_ent *ent, hashtbl_t *n_ways){
	case_ent out = {};
	out.left = ent->left;
	out.right = ent->right;
	for(uint64_t i = 0; i < 7; ++i){
		uint64_t size, mult;
		case_ent_get_part(ent, i, &size, &mult);
		if(!mult){
			break;
		}
		for(uint64_t j = 0; j < size/2; ++j){
			if(j){
				split_part(&out, ent, i, j);
				out.m_here = ent->m_here + 1;
				out.m_max = out.m_here > ent->m_max ? out.m_here : ent->m_max;
			}else{
				shorten_part(&out, ent, i);
				out.m_here = ent->m_here;
				out.m_max = ent->m_max;
			}
			int status;
			out.count = 2*ent->count*mult;
			//check_case_ent(ent, &out);
			hash_append(n_ways, &ft, &out, &status);
			//TODO: check status
		}
		if(size&1){
			if(size == 1){
				remove_part(&out, ent, i);
				out.m_here = ent->m_here - 1;
				out.m_max = ent->m_max;
			}else{
				bisect_part(&out, ent, i, size/2);
				out.m_here = ent->m_here + 1;
				out.m_max = out.m_here > ent->m_max ? out.m_here : ent->m_max;
			}
			int status;
			out.count = ent->count*mult;
			//check_case_ent(ent, &out);
			hash_append(n_ways, &ft, &out, &status);
			//TODO: check status
		}
	}
}

void next_ways(hashtbl_t *ways, hashtbl_t *n_ways){
	case_ent out = {};
	for(case_ent *ent = hash_next(ways, &ft, NULL); ent; ent = hash_next(ways, &ft, ent)){
		//print_case_ent(ent);
		for(uint64_t i = 0; i < ent->left; ++i){
			if(ent->left - i - 1){
				add_part(&out, ent, ent->left - i - 1);
				out.m_here = ent->m_here + 1;
				out.m_max = out.m_here > ent->m_max ? out.m_here : ent->m_max;
			}else{
				out = *ent;
			}
			out.count = ent->count;
			out.left = i;
			out.right = ent->right;
			int status;
			//check_case_ent(ent, &out);
			hash_append(n_ways, &ft, &out, &status);
			//TODO: check status
		}
		for(uint64_t i = 0; i < ent->right; ++i){
			if(ent->right - i - 1){
				add_part(&out, ent, ent->right - i - 1);
				out.m_here = ent->m_here + 1;
				out.m_max = out.m_here > ent->m_max ? out.m_here : ent->m_max;
			}else{
				out = *ent;
			}
			out.count = ent->count;
			out.right = i;
			out.left = ent->left;
			int status;
			//check_case_ent(ent, &out);
			hash_append(n_ways, &ft, &out, &status);
			//TODO: check status
		}
		process_inner_unselected(ent, n_ways);
	}
}

static const uint64_t N = 40;
const uint64_t case_ent_reserve_len = 40000000;
//const uint64_t case_ent_reserve_len = 400000;

void init_ways(hashtbl_t *ways){
	//(left:1, right:0, m_here:1, m_max:1, partition:()): 2.000000
	//(left:0, right:1, m_here:1, m_max:1, partition:()): 2.000000
	//(left:0, right:0, m_here:2, m_max:2, partition:(1,)): 2.000000
	case_ent ent = {.m_here=1,.m_max=1,
		.count=1,
		//.size1=1, .count1=1
	};
	int status;
	//hash_insert(ways, &ft, &ent, &status);
	
	for(uint64_t i = 0; i < N; ++i){
		ent.left = N - i - 1;
		ent.right = i;
		hash_insert(ways, &ft, &ent, &status);
		//TODO: check status
	}
	
	/*
	if(N&1){
		ent.left = ent.right = N/2;
		ent.count = 1;
		hash_insert(ways, &ft, &ent, &status);
		//TODO: check status
	}
	*/
}

int main(){
	
	hashtbl_t _ways1, _ways2, *ways1 = &_ways1, *ways2 = &_ways2, *ways_tmp;
	if(!hash_init(ways1, &ft, case_ent_reserve_len) || !hash_init(ways2, &ft, case_ent_reserve_len)){
		printf("Ooof out of memory\n");
		exit(EXIT_FAILURE);
	}
	init_ways(ways1);
	for(uint64_t t = 1; t < N; ++t){
		printf("%"PRIu64" current states\nQuantifying states with %"PRIu64" tiles\n", ways1->full, t + 1);
		next_ways(ways1, ways2);
		hash_clear(ways1, &ft);
		ways_tmp = ways2;
		ways2 = ways1;
		ways1 = ways_tmp;
	}
	hash_destroy(ways2, &ft);
	printf("Final table:\n");
	double net_m = 0, net_count;
	for(case_ent *ent = hash_next(ways1, &ft, NULL); ent; ent = hash_next(ways1, &ft, ent)){
		//printf("%"PRIu64": %f\n", (uint64_t)ent->m_max, ent->count);
		print_case_ent(ent);
		net_m += ent->count*ent->m_max;
		net_count += ent->count;
	}
	hash_destroy(ways1, &ft);
	printf("E(M)=%.10f\n", net_m/net_count);
	
	/*
	case_ent ent = {.left=2, .m_here=3, .m_max=3, .size1=2, .count1=1, .size2=3, .count2=1, .count=12}, out = {.left=2};
	bisect_part(&out, &ent, 1, 1);
	check_case_ent(&ent, &out);
	*/
}

