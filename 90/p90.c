#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 01,04,06,16,46 must happen
 * 36,18 are garanteed by the selection of 3 and 8
 * 25 is garanteed by the selection of 2 and 5
 * Each cube must have one of the following sets at minimum: 06 014 016 046 146
 * Here is a grid of the sets that work (lower half only):
 * <<^^ -146 0--6 0-46 01-6 014- 0146
 * -146     
 * 0--6 383-     
 * 0-46 383-          
 * 01-6 3838      3-38     
 * 014- 38-8 3--8 3--8 38-8     
 * 0146 3838 3-38 3-38 3838 -838 3838
 */
//5 because one cube always has 2 and the other always has 5
int baseSets[14][2][5] = {
//<<^^   -146                                0--6                                0-46                                01-6                                014-                                0146
/*-146*/
/*0--6*/ {{ 0, 6,-1,-1,-1},{ 1, 4, 6,-1,-1}},
/*0-46*/ {{ 0, 4, 6,-1,-1},{ 1, 4, 6,-1,-1}},
/*01-6*/ {{ 0, 1, 6,-1,-1},{ 1, 4, 6,-1,-1}},                                    {{ 0, 1, 6,-1,-1},{ 0, 4, 6,-1,-1}},
/*014-*/ {{ 0, 1, 4,-1,-1},{ 1, 4, 6,-1,-1}},{{ 0, 1, 4,-1,-1},{ 0, 6,-1,-1,-1}},{{ 0, 1, 4,-1,-1},{ 0, 4, 6,-1,-1}},{{ 0, 1, 4,-1,-1},{ 0, 1, 6,-1,-1}},
/*0146*/ {{ 0, 1, 4, 6,-1},{ 1, 4, 6,-1,-1}},{{ 0, 1, 4, 6,-1},{ 0, 6,-1,-1,-1}},{{ 0, 1, 4, 6,-1},{ 0, 4, 6,-1,-1}},{{ 0, 1, 4, 6,-1},{ 0, 1, 6,-1,-1}},{{ 0, 1, 4, 6,-1},{ 0, 1, 4,-1,-1}},{{ 0, 1, 4, 6,-1},{ 0, 1, 4, 6,-1}},
};

int cubes[3] = {1, 5, 15};

int cubes6[3] = {2, 9, 45};

int notIn(int baseCube[5], int n){
	for(int i = 0; i < 5; ++i){
		if(baseCube[i] == n){
			return 0;
		}
		if(baseCube[i] == -1){
			break;
		}
	}
	return 1;
}

int add(int baseCube[5], int n){
	int i = 2;
	while(baseCube[i] != -1){
		++i;
		if(i >= 5){
			return 0;
		}
	}
	baseCube[i] = n;
	return 1;
}

int countSpaces(int baseCube[5]){
	if(baseCube[4] != -1){
		return 0;
	}
	if(baseCube[3] != -1){
		return 1;
	}
	return 2;
}

int count(int baseCube[5]){
	int spaces = countSpaces(baseCube);
	if(notIn(baseCube, 6)){
		return cubes[spaces];
	}else{
		return cubes6[spaces];
	}
}

int count25(int baseSet[2][5]){
	return count(baseSet[0])*count(baseSet[1]);
}

int count38(int baseSet[2][5], int c3, int c8){
	if(notIn(baseSet[1 - c3], 6)){
		return 0;
	}
	if(notIn(baseSet[1 - c8], 1)){
		return 0;
	}
	int thisSet[2][5];
	memcpy(thisSet, baseSet, 5*2*sizeof(int));
	add(thisSet[c3], 3);
	if(!add(thisSet[c8], 8)){
		return 0;
	}
	return 2*count25(thisSet);
}

int count0146(int baseSet[2][5]){
	return count38(baseSet, 1, 1) + count38(baseSet, 1, 0) + count38(baseSet, 0, 1) + count38(baseSet, 0, 0);
}

int main(){
	int c = 0;
	for(int i = 0; i < 14; ++i){
		c += count0146(baseSets[i]);
	}
	printf("%i\n", c);
}

