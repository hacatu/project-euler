#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Generate all triangles that can be half of
 * an integer angled quadrilateral and sort them
 * into groups by center angle A and by the ratio
 * of the two line segments on the bottom
 *                         .
 *          180 - A - B . .  . C
 *                   .   .     .
 *                .     .        .
 *             .       .           .
 *          . B     A . 180 - A      . A - C
 *       ...............................
 * A - C' . 180 - A . A        B'    .
 *          .      .            .
 *           .    .        .
 *             . .    .
 *         C'   .  180 - A - B'
 * 
 * If we assign the middle line segment a length of
 * 1, the law of sines says the left bottom segment has
 * a length of sin(180 - A - B)/sin(B).  Because
 * sin(x + 180) = -sin(x) and sin(-x) = -sin(x), this is
 * the same as sin(A + B)/sin(B).  The right bottom segment
 * has a length of sin(C)/sin(A - C).  So the ratio of the
 * two bottom segments is sin(B)sin(C):sin(A + B)sin(A - C)
 * A is on the
 * range [2, 90], B is on the range [1, 89], and C is on the
 * range [1, A - 1].  Therefore, A + B is on the range [2, 179]
 * and A - C is on the range [1, 89] and so sin(A + B)sin(C) is
 * always positive and so is sin(A - C)sin(B).  The minimum
 * difference between two such values would be at least
 * (1 - sin(89))*sin(1) ~= 2.658086e-6.
 * 
 * We will use a list of A, B, C, ratio tuples sorted by ratio,
 * A, B, then C.  For every ratio and A, all quadrilaterals with
 * a triangle like that also have a triangle with the inverse
 * ratio and the same angle.
 * 
 * What about symmetry?  Most quadrilaterals are part of a group
 * of 8 similar quadrilaterals: four by rotation times two by
 * reflection.  Restricting A <= 90 handles reflection, so now a
 * quadrilateral may be counted 4 times.  B must be the smallest
 * angle in its rotational group of angles.  If there are three
 * angles in that group that are the same, they are all the same
 * and it does not really matter (this can only happen when A is
 * 90).  If there are only two the same, then they are opposite
 * and it does not really matter.  When finding something by ratio,
 * we use binary search, but within groups by ratio, linear
 * search should probably be used since the lists are short.
 * 
 * So anyway, the symmetry class of a quadrilateral is D_4, D_2, Z_4, Z_2, or Z_1.
 * Case Z_1 (square):
 *  A == 90, B == 180 - A - B == C == A - C == B' == 180 - A - B' == C' == A - C', l == l' == 1
 * Case Z_2 (rhombus):
 *  A == 90, B == A - C == B' == A - C', 180 - A - B == C == 180 - A - B' == C', l == l' == 1
 *  only count the orientation with B < 180 - A - B
 * Case Z_2 (rectangle):
 * 	A < 90, B == 180 - A - B == B' == 180 - A - B', C == A - C == C' == A - C', l == l' == 1
 *  only count the orientation with A < 90
 * Case Z_4 (kite):
 *  A == 90, B == A - C, 180 - A - B == C, B == A - C', 180 - A - B' == C', l == l' == 1
 *  only count the orientation with B < 180 - A - B, B == A - C
 * Case Z_4 (short isosceles trapezoid):
 *  A < 90, B == 180 - A - B', 180 - A - B == B', C == A - C, C' == A - C'
 *  only count the orientation where A < 90, B < 180 - A - B
 * Case Z_4 (tall isosceles trapezoid):
 *  A < 90, B == 180 - A - B == B' == 180 - A - B', C == A - C', A - C == C'
 * Case Z_4 (square isosceles trapezoid):
 *  A == 90, 
 * Case D_2 (parallelogram):
 *  A < 90, B == B', 180 - A - B == 180 - A - B', C == C', A - C == A - C', l == l' == 1
 *  only count the orientation where A < 90, B < 180 - A - B
 * Case D_4 (irregular):
 *  <restrictions which don't fit any other category>
 *  only count the orientation where A < 90, B < (180 - A - B, B', 180 - A - B')
 *   this warrents a comment, because even in the irregular case we could have B == 180 - A - B,
 *   but then B' and 180 - A - B' are not equal to B or each other so one must be smaller than B if the angles are not all distinct
 * 
 * So we can see we can always require A <= 90, B <= 180 - A - B
 * If A == 90 (square, rhombus, kite, square isosceles trapezoid, irregular):
 *  If B == 180 - A - B (square, square isosceles trapezoid, kite, irregular):
 *   If B == C (square, kite):
 *    we have at least one comp triangle (the identical one forming a square)
 *    among comp triangles, all will have B' == A - C' and we don't need to check, so we just need to check B == B' or not to see if we have a square
 *   Else (square isosceles trapezoid, kite, irregular):
 *    we have at least one comp triangle (the one forming a square isosceles trapezoid)
 *    If B == B' (square isosceles trapezoid):
 *     we have a square isosceles trapezoid
 *    Else If B == C' (kite):
 *     the kite is not counted in this orientation
 *    Else (irregular):
 *     we have an irregular shape, ensure B is minimal in its orbit
 *  Else If B == A - C (rhombus, kite):
 *   we have at least one comp triangle (the identical one forming a rhombus)
 *   among comp triangles, all will have B' == A - C' and we don't need to check, so we just need to check B == B' or not to see if we have a rhombus
 *  Else (kite, irregular):
 *   we have at least one comp triangle (the one forming a kite in the wrong orientation)
 *   If B == A - C' (kite):
 *    the kite is not counted in this orientation
 *   Else (irregular):
 *    we have an irregular shape, ensure B is minimal in its orbit
 * Else A < 90 (rectangle, short isosceles trapezoid, tall isosceles trapezoid, parallelogram, irregular):
 *  If B == 180 - A - B (rectangle, tall isosceles trapezoid, irregular):
 *   If C == A - C (rectangle, irregular):
 *    we have at least one comp triangle (the one forming a rectangle)
 *    If B' == B (rectangle):
 *     we have a rectangle
 *    Else (irregular):
 *     we have an irregular shape, ensure B is minimal in its orbit
 *   Else (tall isosceles trapezoid, irregular):
 *    we have at least one comp triagle (the one forming a tall isosceles trapezoid)
 *    If B' == B (tall isosceles trapezoid):
 *     we have a tall isosceles trapezoid
 *    Else (irregular):
 *     we have an irregular shape, ensure B is minimal in its orbit
 *  Else B < 180 - A - B (short isosceles trapezoid, irregular, parallelogram):
 *   If C == A - C (short isosceles trapezoid, irregular):
 *    we have at least one comp triangle (the one forming a short isosceles trapezoid)
 *    If C == C' (short isosceles trapezoid):
 *     we have a short isosceles trapezoid
 *    Else (irregular):
 *     we have an irregular shape, ensure B is minimal in its orbit
 *   Else (irregular, parallelogram):
 *    we have at least one comp triangle (the one forming a parallelogram)
 *    If B == B', C == C' (parallelogram):
 *     we have a parallelogram
 *    Else (irregular):
 *     we have an irregular shape, ensure B is minimal in its orbit
 *   Else (!!!):
 *    this is the B > 180 - A - B case which is excluded.
 * 
 * This is a fairly complicated decision procedure and it might be simpler if we forced B to be minimal in its orbit first
 */

#define TRIANGLES_LEN (89*45*89)
#define TRIANGLE_EPSILON 1.e-6 //This is less than the 2.645086e-6 calculated before

typedef struct{
	int A, B, C;
	long double ratio;
} triangle_t;

long double sin_table[180];
triangle_t triangles[89*45*89];
int i;

int cmp_triangles(const void *_a, const void *_b){
	const triangle_t *a = _a, *b = _b;
	long double ord = a->ratio - b->ratio;
	if(ord < -TRIANGLE_EPSILON){
		return -1;
	}else if(ord > TRIANGLE_EPSILON){
		return +1;
	}else if(a->A < b->A){//note that a stable sort would obviate this and later checks
		return -1;
	}else if(a->A > b->A){
		return +1;
	}else if(a->B < b->B){
		return -1;
	}else if(a->B > b->B){
		return +1;
	}else if(a->C < b->C){
		return -1;
	}else if(a->C > b->C){
		return +1;
	}
	return 0;
}

//find the index of the first triangle with the same ratio as the triangle at a given index
int first_of(int i){
	while(i && triangles[i].ratio - triangles[i - 1].ratio <= TRIANGLE_EPSILON){
		--i;
	}
	return i;
}

//find the index of the first triangle with a different A than the triangle at a given index i, or return j if there is not one before j
int next_A(int i, int j){
	do{
		++i;
	}while(i < j && triangles[i].A == triangles[i - 1].A);
	return i;
}

//ensure that B <= C for the current triangle
int do_check_upper(void){
	return triangles[i].B <= triangles[i].C;
}

int do_check_lower(int j){
	return !!(triangles[i].B <= triangles[j].B && triangles[i].B <= triangles[j].C);
}

int do_A(int j_A, int j_end){
	int count = 0;
	do{
		if(!do_check_upper()){
			break;
		}
		for(int j = j_A; j < j_end; ++j){
			count += do_check_lower(j);
		}
		++i;
	}while(i < TRIANGLES_LEN && triangles[i].A == triangles[i - 1].A && triangles[i].ratio - triangles[i - 1].ratio <= TRIANGLE_EPSILON);
	return count;
}

int do_i_ratio(int j_ratio, int j_end){
	int j_A = j_ratio, j_next, count = 0;
	while(j_A < j_end){
		j_next = next_A(j_A, j_end);
		if(triangles[i].A < triangles[j_A].A){
			++i;
		}else if(triangles[i].A > triangles[j_A].A){
			j_A = j_next;
		}else{
			count += do_A(j_A, j_next);
		}
		if(triangles[i].ratio - triangles[i - 1].ratio > TRIANGLE_EPSILON){
			return count;
		}
	}
	while(triangles[i + 1].ratio - triangles[i].ratio <= TRIANGLE_EPSILON){
		++i;
	}
	return count;
}

int main(void){
	printf("computing sine table ...\n");
	for(int O = 1; O <= 179; ++O){
		sin_table[O] = sinl(O*M_PI/180);
	}
	printf("enumerating triangles ...\n");
	for(int A = 2, i = 0; A <= 90; ++A){
		for(int B = 1; B <= 89; ++B){
			for(int C = 1; C <= A - 1; ++C){
				long double ratio = sin_table[B]*sin_table[C]/(sin_table[A + B]*sin_table[A - C]);
				triangles[i++] = (triangle_t){.A=A, .B=B, .C=C, .ratio=ratio};
			}
		}
	}
	printf("sorting triangles ...\n");
	qsort(triangles, TRIANGLES_LEN, sizeof(triangle_t), cmp_triangles);
	int count = 0;
	printf("counting triangle pairs for every ratio ...\n");
	for(int j_end = TRIANGLES_LEN, j_ratio; i < TRIANGLES_LEN; j_end = j_ratio){
		j_ratio = first_of(j_end - 1);
		printf("%d\n", i);
		count += do_i_ratio(j_ratio, j_end);
	}
	printf("%i\n", count);
}

