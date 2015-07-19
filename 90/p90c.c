#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pairs0146[5][2] = {{0,1},{0,4},{0,6},{1,6},{4,6}};

int pairs[3][2] = {{3,6},{1,8},{2,5}};

int digits0146[5] = {0,1,4,6,9};

int digits[5] = {2,3,5,7,8};

int count1sInB(int n){
	int c = 0;
	while(n &= n - 1)++c;
	return c;
}

int checkOp(int cube[6], int p){
	for(int i = 0; i < 6; ++i){
		if(cube[i] == p){
			return 1;
		}
	}
	return 0;
}

int checkPair(int cubes[2][6], int pair[2]){
	for(int i = 0; i < 6; ++i){
		if(cubes[0][i] == pair[0]){
			if(checkOp(cubes[1], pair[1])){
				return 1;
			}
		}else if(cubes[0][i] == pair[1]){
			if(checkOp(cubes[1], pair[0])){
				return 1;
			}
		}
	}
	return 0;
}

int checkAll(int cubes[2][6]){
	for(int i = 0; i < 3; ++i){
		if(!checkPair(cubes, pairs[i])){
			return 0;
		}
	}
	return 1;
}

int f9(int n){return n==9?6:n;}

int check9op(int cube[6], int sides, int p){
	for(int i = 0; i < sides; ++i){
		if(f9(cube[i]) == p){
			return 1;
		}
	}
	return 0;
}

int checkPair9s(int cubes[2][6], int sides[2], int pair[2]){
	for(int i = 0; i < sides[0]; ++i){
		if(f9(cubes[0][i]) == pair[0]){
			if(check9op(cubes[1], sides[1], pair[1])){
				return 1;
			}
		}else if(f9(cubes[0][i]) == pair[1]){
			if(check9op(cubes[1], sides[1], pair[0])){
				return 1;
			}
		}
	}
	return 0;
}

int check1inEach9s(int cube[6], int sides){
	for(int i = 0; i < 5; ++i){
		for(int j = 0; j < sides; ++j){
			if(pairs0146[i][0] == f9(cube[j]) || pairs0146[i][1] == f9(cube[j])){
				goto hasOne;
			}
		}
		return 0;
		hasOne:;
	}
	return 1;
}

int checkAll9s(int cubes[2][6], int sides[2]){
	for(int i = 0; i < 5; ++i){
		if(!checkPair9s(cubes, sides, pairs0146[i])){
			return 0;
		}
	}
	return 1;
}

int countStuff(int cubes[2][6], int sides[2], int i, int j){
	int cubes2[2][6];
	int sides2[2];
	memcpy(cubes2, cubes, sizeof(cubes2));
	memcpy(sides2, sides, sizeof(sides2));
	for(int n = 0b10000, m = 0; n; n >>= 1, ++m){
		if(i&n)cubes2[0][sides2[0]++] = digits[m];
		if(j&n)cubes2[1][sides2[1]++] = digits[m];
	}
	if(!checkAll(cubes2)){
		return 0;
	}
	return 1;
}

int countNot9s(int cubes[2][6], int sides[2]){
	int c = 0;
	for(int i = 0b00000; i <= 0b11111; ++i){
		if(!(i & 0b11000)){//no 2 or 5
			continue;
		}
		if(sides[0] + count1sInB(i) != 6){//wrong number of numbers
			continue;
		}
		for(int j = 0b00000; j <= 0b11111; ++j){
			if(!(j & 0b11000)){//no 2 or 5
				continue;
			}
			if(sides[1] + count1sInB(j) != 6){//wrong number of numbers
				continue;
			}
			c += countStuff(cubes, sides, i, j);
		}
	}
	return c;
}

int main(){
	int cubes[2][6];
	int sides[2];
	int c = 0;
	for(int i = 0b00000; i <= 0b11111; ++i){
		//memset(cubes[0], -1, sizeof(cubes[0]));
		sides[0] = 0;
		for(int n = 0b10000, m = 0; n; n >>= 1, ++m){
			if(i&n)cubes[0][sides[0]++] = digits0146[m];
		}
		if(!check1inEach9s(cubes[0], sides[0])){
			continue;
		}
		for(int j = 0b00000; j <= 0b11111; ++j){
			//memset(cubes[1], -1, sizeof(cubes[1]));
			sides[1] = 0;
			for(int n = 0b10000, m = 0; n; n >>= 1, ++m){
				if(i&n)cubes[1][sides[1]++] = digits0146[m];
			}
			if(!checkAll9s(cubes, sides)){
				continue;
			}
			//all 01469 counted (please)
			c += countNot9s(cubes, sides);
		}
	}
	printf("%i\n", c);
}

