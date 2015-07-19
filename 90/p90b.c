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

int in(int baseCube[5], int n){
	for(int i = 0; i < 5; ++i){
		if(baseCube[i] == n){
			return 1;
		}
		if(baseCube[i] == -1){
			break;
		}
	}
	return 0;
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

int count25(int baseSet[2][5]){//0146387925 are in place
	return 1;
}

int count9(int baseSet[2][5]){//01463879 are in place, 25 are not
	//you may multiply count25 by 2 iff both cubes do not have a 2 or 5.  If one does it is ok.
	int thisSet[2][5];
	int c2 = 0b00;
	int c = 0;
	int m;
	for(;c2 <= 0b11; ++c2){
		memcpy(thisSet, baseSet, sizeof(thisSet));
		m = c2 == 0b11?1:2;
		if(c2 & 0b10){
			if(!add(thisSet[0], 2)){
				continue;
			}
		}
		if(c2 & 0b01){
			if(!add(thisSet[0], 5)){
				continue;
			}
		}
		c += m*count25(thisSet);
	}
	return c;
}

int count7(int baseSet[2][5]){//0146387 are in place, 259 are not.
	int thisSet[2][5];
	int c9 = 0b00;
	int c = 0;
	int m;
	for(;c9 <= 0b11; ++c9){
		memcpy(thisSet, baseSet, sizeof(thisSet));
		m = 1;
		if(c9 & 0b10){
			if(!add(thisSet[0], 9)){
				continue;
			}
			if(!in(thisSet[0], 6)){
				m *= 2;
			}
		}else{
			if(in(thisSet[0], 6)){
				m *= 2;
			}
		}
		if(c9 & 0b01){
			if(!add(thisSet[1], 9)){
				continue;
			}
			if(!in(thisSet[1], 6)){
				m *= 2;
			}
		}else{
			if(in(thisSet[1], 6)){
				m *= 2;
			}
		}
		c += m*count9(thisSet);
	}
	return c;
}

int count38(int baseSet[2][5]){//all of 014638 are in place when this function is called, 7 is not and 25 and 9 are not. There are at most 3 empty spaces
	int thisSet[2][5];
	int c7 = 0b00;
	int c = 0;
	for(;c7 <= 0b11; ++c7){
		memcpy(thisSet, baseSet, sizeof(thisSet));
		if(c7 & 0b10){
			if(!add(thisSet[0], 7)){
				continue;
			}
		}
		if(c7 & 0b01){
			if(!add(thisSet[1], 7)){
				continue;
			}
		}
		c += count7(thisSet);
	}
	return c;
}

int check38(int baseSet[2][5]){
	if(!((in(baseSet[0], 6) && in(baseSet[1], 3)) || (in(baseSet[1], 6) && in(baseSet[0], 3)))){
		return 0;
	}
	if(!((in(baseSet[0], 1) && in(baseSet[1], 8)) || (in(baseSet[1], 1) && in(baseSet[0], 8)))){
		return 0;
	}
	return 1;
}

int count0146(int baseSet[2][5]){//all of 0146 are in place when this function is called
	int thisSet[2][5];
	int c38 = 0b0001;
	int c = 0;
	for(;c38 <= 0b1111; ++c38){
		memcpy(thisSet, baseSet, sizeof(thisSet));
		if(c38 & 0b1000){
			add(thisSet[0], 3);
		}
		if(c38 & 0b0100){
			if(!add(thisSet[0], 8)){
				continue;
			}
		}
		if(c38 & 0b0010){
			add(thisSet[1], 3);
		}
		if(c38 & 0b0001){
			if(!add(thisSet[0], 8)){
				continue;
			}
		}
		if(!check38(thisSet)){
			continue;
		}
		c += count38(thisSet);
	}
	return c;
}

int main(){
	int c = 0;
	for(int i = 0; i < 14; ++i){
		c += count0146(baseSets[i]);
	}
	printf("%i\n", c);
}

