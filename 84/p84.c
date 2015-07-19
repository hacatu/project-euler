#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GO 0
#define JAIL 10
#define GO_TO_JAIL 30
#define CC1 2
#define CC2 17
#define CC3 33
#define CH1 7
#define CH2 22
#define CH3 36
#define U1 12
#define U2 28
#define R1 5
#define R2 15
#define R3 25
#define R4 35

char roll(){
	static char bits;
	static int r;
	int temp;
	if(bits == 0){
		r = rand();
		bits = 32;
	}
	bits -= 2;
	temp = r&3;
	r >>= 2;
	return 1 + temp;
}

char draw(){
	static char bits;
	static int r;
	int temp;
	if(bits == 0){
		r = rand();
		bits = 32;
	}
	bits -= 4;
	temp = r&15;
	r >>= 4;
	return temp;
}

int main(int argc, char **argv){
	if(argc < 2){
		puts("Expected number of runs");
		return 1;
	}
	int runs = atoi(argv[1]);
	int space = GO;
	int spaces[40] = {0};
	int doubles = 0;
	int roll1, roll2;
	srand(time(NULL));
	for(int run = 0; run < runs; ++run){
		roll1 = roll();
		roll2 = roll();
		if(roll1 == roll2){
			++doubles;
			if(doubles == 3){
				doubles = 0;
				space = JAIL;
			}
		}else{
			doubles = 0;
			space += roll1 + roll2;
			space %= 40;
			//LABEL FOR LATER GOTO:
			special_movement:
			//This is needed if the player moves onto some space with special movement, to analyze special movement again.
			switch(space){
				case GO_TO_JAIL:
				space = JAIL;
				break;
				case CC1:
				case CC2:
				case CC3:
				roll1 = draw();
				switch(roll1){
					case 0:
					space = GO;
					break;
					case 1:
					space = JAIL;
					break;
				}
				break;
				case CH1:
				case CH2:
				case CH3:
				roll1 = draw();
				switch(roll1){
					case 0://Go
					space = GO;
					break;
					case 1://JAIL
					space = JAIL;
					break;
					case 2:
					space = 11;
					break;
					case 3:
					space = 24;
					break;
					case 4:
					space = 49;
					break;
					case 5:
					space = R1;
					break;
					case 6:
					case 7://next r
					switch(space){
						case CH1:
						space = R2;
						break;
						case CH2:
						space = R3;
						break;
						case CH3:
						space = R1;
						break;
					}
					break;
					case 8://next u
					switch(space){
						case CH1:
						space = U1;
						break;
						case CH2:
						space = U2;
						break;
						case CH3:
						space = U1;
						break;
					}
					break;
					case 9:
					space -= 3;
					goto special_movement;
					break;
				}
				break;
			}
		}
		++spaces[space];
	}
	for(int space = 0; space < 40; ++space){
		printf("P(%2i) = %f\n", space, (double)spaces[space]/(double)runs);
	}
}
