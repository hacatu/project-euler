#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <crater/hash.h>

#define VEC_ISORT_BOUND 16

typedef int64_t i64_3[3];
typedef struct i64_3pq i64_3pq;
struct i64_3pq{
	i64_3 point;
	double dist;
	i64_3pq *first_child, *next_sibling, *parent;
};

static inline uint64_t hash_i64_3pq(const cr8r_base_ft *ft, const void *_self){
	const i64_3pq *self = _self;
	return self->point[0]^(self->point[1]*101)^(self->point[2]*47);
}

static inline int cmp_i64_3pq(const cr8r_base_ft *ft, const void *_a, const void *_b){
	const i64_3pq *a = _a, *b = _b;
	for(uint64_t i = 0; i < 3; ++i){
		if(a->point[i] < b->point[i]){
			return -1;
		}else if(a->point[i] > b->point[i]){
			return 1;
		}
	}
	return 0;
}

cr8r_hashtbl_ft i64_3pqft = {
	.base={
		.size=sizeof(i64_3pq)
	},
	.hash=hash_i64_3pq,
	.cmp=cmp_i64_3pq,
	.load_factor=.7
};

/*
static inline int cmp_i64_3x(const void *_a, const void *_b){
	const int64_t *a = _a, *b = _b;
	if(a[0] < b[0]){
		return -1;
	}else if(a[0] > b[0]){
		return 1;
	}
	return 0;
}
*/

static inline int cmp_i64_3y(const void *_a, const void *_b){
	const int64_t *a = _a, *b = _b;
	if(a[1] < b[1]){
		return -1;
	}else if(a[1] > b[1]){
		return 1;
	}
	return 0;
}

/*
static inline int cmp_i64_3z(const void *_a, const void *_b){
	const int64_t *a = _a, *b = _b;
	if(a[2] < b[2]){
		return -1;
	}else if(a[2] > b[2]){
		return 1;
	}
	return 0;
}
*/

static inline int cmp_i64_3ya(const void *_a, const void *_b){
	const int64_t *a = _a, *b = _b;
	if((a[2] < 0) == (b[2] < 0)){
		int64_t ba = b[0]*a[2];
		int64_t ab = a[0]*b[2];
		if(ba < ab){
			return -1;
		}else if(ba == ab){
			return 0;
		}
		return 1;
	}
	return a[2] < 0 ? -1 : 1;
}

static inline void swap(void *a, void *b, uint64_t size){
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}

// Insertion sort the range of elements [a, a + stride, a + 2*stride, ..., b).
// Note that b is exlusive.
static inline void range_sort_i(void *base, uint64_t size, uint64_t a, uint64_t b, uint64_t stride, int (*cmp)(const void*, const void*)){
	void *start = base + a*size;
	void *end = base + b*size;
	uint64_t offset = stride*size;
	for(void *it = start + offset; it < end; it += offset){
		for(void *jt = it; jt > start && cmp(jt - offset, jt) > 0; jt -= offset){
			swap(jt - offset, jt, size);
		}
	}
}

void *pivot_mm(void *base, uint64_t size, uint64_t a, uint64_t b, int (*cmp)(const void*, const void*)){
	if(a >= b){
		return NULL;
	}
	// the traditional median of medians algorithm works by viewing an array in chunks of 5 elements,
	// creating a new array made up of the medians of these chunks, and recursing.
	// since each chunk has 5 elements, its median can be found in constant time by sorting.
	// the lengths of the arrays at each step form a geometric series, which has a finite sum,
	// in particular, for 5 element chunks the total length of all arrays processed when recursing is 5/4 times
	// the length of the original array.
	//
	// instead of allocating any new array to hold the medians of chunks, we can sort each chunk in place in the
	// big array and then do a couple tricks.  First, we swap the middle and middle elements of each chunk,
	// so that the medians are all at the fronts.  Then, we can work on the array of medians in place
	// by treating the first elements of all the chunks as a strided array.  We start with an array of elements
	// grouped into chunks of 5, then we have an array with stride 5 of medians of chunks, then we have
	// an array with stride 25 of medians of medians of chunks, and so on.
	// look carefully at how incomplete chunks are dealt with: we simply find the median of <5 elements if the last
	// chunk is incomplete. 
	for(uint64_t chunk_size = 5, chunk_stride = 1; chunk_stride < b - a; chunk_stride = chunk_size, chunk_size *= 5){
		for(uint64_t i = a; i < b; i += chunk_size){
			uint64_t curr_size = i + chunk_size > b ? b - i : chunk_size;
			uint64_t curr_len = (curr_size + chunk_stride - 1)/chunk_stride;
			if(curr_len == 1){
				break;
			}
			range_sort_i(base, size, i, curr_size, chunk_stride, cmp);
			if(curr_len == 2){
				break;
			}
			swap(base + i*size, base + (i + (curr_len - 1)/2*chunk_stride)*size, size);
		}
	}
	// at the end, the first element is the median of medians
	return base + a*size;
}

void *pivot_m3(void *base, uint64_t size, uint64_t a, uint64_t b, int (*cmp)(const void*, const void*)){
	if(a >= b){
		return NULL;
	}
	void *fst = base + a*size;
	void *lst = base + (b - 1)*size;
	void *mid = base + (a + b - 1)/2*size;
	int ord = cmp(fst, mid);
	int sorted;
	if(!ord){
		return mid;
	}else if((sorted = ord*cmp(mid, lst))){
		if(sorted > 0){
			return mid;
		}else if(cmp(lst, fst)*ord >= 0){
			return lst;
		}else{
			return fst;
		}
	}else{
		return mid;
	}
}

void *partition(void *base, uint64_t size, uint64_t a, uint64_t b, void *piv, int (*cmp)(const void*, const void*)){
	if(a >= b){
		return NULL;
	}
	void *fst = base + a*size;
	void *lst = base + (b - 1)*size;
	if(piv < fst || piv > lst){
		return NULL;
	}
	if(piv != lst){
		swap(piv, lst, size);
	}
	// now, lst points to the pivot
	void *it = fst - size;
	void *jt = lst;
	// initially, *it is one before the start of the subrange, so that after
	// adding 1 to it, all elements preceding it (ie no elements) will be
	// known to be < pivot.
	// on the other hand, *jt points to the pivot so all elements beginning
	// at *jt are trivially >= the pivot (since *jt points to the last element
	// initially and the last element holds the pivot)
	while(1){
		do{
			it += size;
		}while(it < jt && cmp(it, lst) < 0);
		// now, *it >= pivot but the elements preceding *it are < pivot
		if(it == jt){
			break;
		}
		do{
			// currently, *jt and subsequent elements are all >= pivot
			jt -= size;
		}while(it < jt && cmp(jt, lst) >= 0);
		if(it == jt){
			break;
		}
		swap(it, jt, size);
	}
	// *jt and subsequent elements are >= pivot, so partitioning
	// is complete and *jt is the first element >= pivot
	if(jt != lst){
		// we need to ensure the pivot is placed at the beginning of the
		// >= pivot region, so we have a (possibly empty) < pivot region,
		// the pivot, and a (possibly empty) >= pivot region
		swap(jt, lst, size);
	}
	return jt;
}

// Partially sort a subrange of the given vector to find an element near the max or min.
// If i < 0, find the (b + i)th element by partially sorting the -i largest elements
// descending in the beginning of the subrange and returning a pointer to the -i largest.
// If i >= 0, find the ith element by partially sorting the i+1 smallest elements ascending
// in the beginning of the subrange and returning a pointer to the i+1 smallest.
static inline void *vec_sort_end(void *base, uint64_t size, uint64_t a, uint64_t b, int64_t i, int (*cmp)(const void*, const void*)){
	// TODO: when i == 0 or -1, this reduces to finding the min/max
	int ord = 1;
	if(i < 0){
		ord = -1;
		i = -i - 1;
	}
	void *start = base + a*size;
	void *res = start + i*size;
	void *end = base + b*size;
	if(res >= end){
		return NULL;// error
	}
	for(void *it = start + size; it <= res; it += size){
		for(void *jt = it; jt > start && ord*cmp(jt - size, jt) < 0; jt -= size){
			swap(jt - size, jt, size);
		}
	}
	for(void *it = res + size; it < end; it += size){
		if(ord*cmp(res, it) <= 0){
			continue;
		}
		swap(res, it, size);
		for(void *jt = res; jt > start && ord*cmp(jt - size, jt) < 0; jt -= size){
			swap(jt - size, jt, size);
		}
	}
	return res;
}

void *vec_ith(void *base, uint64_t size, uint64_t a, uint64_t b, uint64_t i, int (*cmp)(const void*, const void*)){
	while(1){
		if(i >= b - a){
			return NULL;
		}else if(i < VEC_ISORT_BOUND){
			return vec_sort_end(base, size, a, b, i, cmp);
		}else if(b - i <= VEC_ISORT_BOUND){
			return vec_sort_end(base, size, a, b, (int64_t)i - (int64_t)b, cmp);
		}
		void *piv = pivot_m3(base, size, a, b, cmp);
		piv = partition(base, size, a, b, piv, cmp);
		if(!piv){
			return NULL;
		}
		uint64_t j = (piv - base)/size - a;
		if(i < j){
			b = j;
		}else if(i > j){
			a = j + 1;
		}else{
			return piv;
		}
	}
}

static inline void kd_s2i64_ify_r(i64_3 *points, uint64_t a, uint64_t b, uint64_t depth){
	while(b > a){
		uint64_t mid_idx = (a + b)/2;
		int (*cmp)(const void*, const void*) = depth&1 ? cmp_i64_3y : cmp_i64_3ya;
		void *piv = vec_ith(points, sizeof(i64_3), a, b, mid_idx, cmp);
		piv = partition(points, sizeof(i64_3), a, b, piv, cmp);
		kd_s2i64_ify_r(points, mid_idx + 1, b, ++depth);
		b = mid_idx;
	}
}

typedef struct{
	i64_3 tl, br;
} s2i64_window;

static inline uint64_t sqdist(const i64_3 *a, const i64_3 *b){
	uint64_t res = 0;
	for(uint64_t i = 0; i < 3; ++i){
		res += ((*a)[0] - (*b)[0])*((*a)[0] - (*b)[0]);
	}
	return res;
}

static inline void split_window(const s2i64_window *self, s2i64_window *subwindows, const i64_3 *root, uint64_t depth){
	memcpy(subwindows[0].tl, self->tl, sizeof(i64_3));
	memcpy(subwindows[1].br, self->br, sizeof(i64_3));
	if(depth&1){// compare by y so split top and bottom
		// subwindows[0].br = (self.br.x, root.y, self.br.z)
		subwindows[0].br[0] = self->br[0];
		subwindows[0].br[1] = (*root)[1];
		subwindows[0].br[2] = self->br[2];
		// subwindows[1].tl = (self.tl.x, root.y, self.tl.z)
		subwindows[1].tl[0] = self->tl[0];
		subwindows[1].tl[1] = (*root)[1];
		subwindows[1].tl[2] = self->tl[2];
	}else{// otherwise we split left and right
		// subwindows[0].br = (root.x, self.br.y, root.z)
		subwindows[0].br[0] = (*root)[0];
		subwindows[0].br[1] = self->br[1];
		subwindows[0].br[2] = (*root)[2];
		// subwindows[1].tl = (root.x, self.tl.y, root.z)
		subwindows[1].tl[0] = (*root)[0];
		subwindows[1].tl[1] = self->tl[1];
		subwindows[1].tl[2] = (*root)[2];
	}
}

//TODO: we don't just want to be able to find the nearest point, we want to be able to find the k nearest points, meaning we will have a heap or bst of points
//found sorted by distance from the query
static inline void kd_s2i64_nearest_r(i64_3 *points, uint64_t a, uint64_t b, const i64_3 *pt, const s2i64_window *window, uint64_t depth, uint64_t *_idx, uint64_t *_sqdist){
	if(a == b){
		return;
	}// TODO: check if pt can possibly be closer to a point in window
	uint64_t mid_idx = (a + b)/2;
	int (*cmp)(const void*, const void*) = depth&1 ? cmp_i64_3y : cmp_i64_3ya;
	void *root = vec_ith(points, sizeof(i64_3), a, b, mid_idx, cmp);
	int ord = cmp(root, pt);
	if(!ord){
		cmp = depth%1 ? cmp_i64_3ya : cmp_i64_3y;
		if(!cmp(root, pt)){
			*_sqdist = 0;
			*_idx = mid_idx;
			return;
		}
		uint64_t sqd = sqdist(root, pt);
		if(sqd < *_sqdist){
			*_sqdist = sqd;
			*_idx = mid_idx;
		}
	}
	s2i64_window subwindows[2];
	split_window(window, subwindows, root, depth);
	kd_s2i64_nearest_r(points, a, mid_idx, pt, subwindows + 0, depth + 1, _idx, _sqdist);
	kd_s2i64_nearest_r(points, mid_idx, b, pt, subwindows + 1, depth + 1, _idx, _sqdist);
}

i64_3 *sphere_latt_points_sqrt(uint64_t r, uint64_t *_num_points){
	if(!r){
		*_num_points = 1;
		return calloc(1, sizeof(i64_3));
	}
	uint64_t cap = r, num_points = 0;
	i64_3 *points = malloc(cap*sizeof(i64_3));
	if(!points){
		return NULL;
	}
	points[num_points][0] = r;
	points[num_points][1] = 0;
	points[num_points++][2] = 0;
	for(uint64_t a = r - 1; 3*a*a >= r*r; --a){
		uint64_t ub = sqrt(r*r - a*a);
		if(a < ub){
			ub = a;
		}
		for(uint64_t b = ub; a*a + 2*b*b >= r*r; --b){
			uint64_t c = sqrt(r*r - a*a - b*b);
			if(c*c == r*r - a*a - b*b){
				if(num_points == cap){
					cap *= 2;
					void *tmp = realloc(points, cap*sizeof(i64_3));
					if(!tmp){
						free(points);
						return NULL;
					}
					points = tmp;
				}
				points[num_points][0] = a;
				points[num_points][1] = b;
				points[num_points++][2] = c;
			}
			if(!b){
				break;
			}
		}
	}
	*_num_points = num_points;
	return realloc(points, num_points*sizeof(i64_3)) ?: points;
}

int main(){
	uint64_t r = (1ull << 4) - 1;
	size_t num_canon_points;
	i64_3 *points = sphere_latt_points_sqrt(r, &num_canon_points);
	if(!points){
		fprintf(stderr, "\e[1;31mCould not find lattice points on sphere with radius %"PRIi64"\e[0m\n", r);
		return 0;
	}
	fprintf(stderr, "\e[1;34mFound %"PRIi64" lattice points in canonical vigiquadrant\e[0m\n", num_canon_points);
	/*
	fprintf(stderr, "\e[1;34m");
	for(uint64_t i = 0; i < num_canon_points; ++i){
		fprintf(stderr, "(%"PRIi64", %"PRIi64", %"PRIi64")", points[i][0], points[i][1], points[i][2]);
		fprintf(stderr, i == num_canon_points - 1 ? "\e[0m\n" : ", ");
	}*/
	void *tmp = realloc(points, 3*num_canon_points*sizeof(i64_3));
	if(!tmp){
		free(points);
		fprintf(stderr, "\e[1;31mCould not extend buffer\e[0m\n");
		return 0;
	}
	points = tmp;
	size_t num_points = num_canon_points;
	for(size_t i = 1; i < num_canon_points; ++i){
		if(points[i][0] != points[i][1]){
			//first add (y, x, z)
			points[num_points][0] = points[i][1];
			points[num_points][1] = points[i][0];
			points[num_points++][2] = points[i][2];
		}
		if(points[i][1] != points[i][2]){
			//now add (z, x, y)
			points[num_points][0] = points[i][2];
			points[num_points][1] = points[i][0];
			points[num_points++][2] = points[i][1];
		}
	}
	//add last point
	points[num_points][0] = 0;
	points[num_points][1] = r;
	points[num_points++][2] = 0;
	points = realloc(points, num_points*sizeof(i64_3)) ?: points;
	
	kd_s2i64_ify_r(points, 0, num_points, 0);

	free(points);
}

