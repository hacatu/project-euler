#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int digitMask[10] = {0b111111111, 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1<<8};

int digit(int n){
	switch(n){
		case 1:return 1;
		case 1<<1:return 2;
		case 1<<2:return 3;
		case 1<<3:return 4;
		case 1<<4:return 5;
		case 1<<5:return 6;
		case 1<<6:return 7;
		case 1<<7:return 8;
		case 1<<8:return 9;
		default:return 0;
	}
}

int isPower2(int n){//do not call on 0
	return !(n&(n - 1));
}

int readBoard(FILE *file, int *board){
	int row;
	for(int i = 0; i < 9; ++i){
		if(fscanf(file, "%d", &row) <= 0){
			return 0;
		}
		for(int j = 8; j >= 0; --j){
			board[9*i + j] = digitMask[row%10];
			row /= 10;
		}
	}
	return 1;
}

void printBoard(int *board){
	for(int row = 0; row < 9; ++row){
		for(int column = 0; column < 9; ++column){
			printf("%i", digit(board[9*row + column]));
		}
		puts("");
	}
}

void getNeighbors(int cell, int neighbors[20]){
	int i = 0, row = cell/9, column = cell%9;
	int baseRow = 3*(row/3), baseColumn = 3*(column/3);
	for(int j = 0; j < 9; ++j){
		if(j == column){
			continue;
		}
		neighbors[i++] = 9*row + j;
	}
	for(int j = 0; j < 9; ++j){
		if(j == row){
			continue;
		}
		neighbors[i++] = 9*j + column;
	}
	for(int cellRow = 0; cellRow < 3; ++cellRow){
		for(int cellColumn = 0; cellColumn < 3; ++cellColumn){//none of these names even make sense!
			if(row%3 != cellRow && column%3 != cellColumn){
				neighbors[i++] = 9*(baseRow + cellRow) + (baseColumn + cellColumn);
			}
		}
	}
}

int eliminateNeighbors(int *board, int cell, int *left){
	int valueMask = ~board[cell], temp;
	int neighbors[20];//8 in row + 8 in column + 4 in cell
	getNeighbors(cell, neighbors);
	for(int i = 0; i < 20; ++i){
		temp = board[neighbors[i]]&valueMask;
		if(temp == board[neighbors[i]]){
			continue;
		}
		board[neighbors[i]] = temp;
		if(!board[neighbors[i]]){
			return 0;
		}
		if(isPower2(board[neighbors[i]])){
			if(!eliminateNeighbors(board, neighbors[i], left)){
				return 0;
			}
			--*left;
			if(!*left){
				return 1;
			}
		}
	}
	return 1;
}

int selectLoneNumbers(int *board, int *left){
	int first;
	for(int i = 1; i <= 9; ++i){
		for(int row = 0; row < 9; ++row){
			first = -1;
			for(int column = 0; column < 9; ++column){
				if(board[9*row + column]&digitMask[i]){
					if(first != -1){
						goto doneRow;
					}
					first = 9*row + column;
				}
			}
			if(first != -1){
				if(board[first] != digitMask[i]){
					board[first] = digitMask[i];
					if(!eliminateNeighbors(board, first, left)){
						return 0;
					}
					--*left;
				}
			}
			doneRow:;
		}
		for(int column = 0; column < 9; ++column){
			first = -1;
			for(int row = 0; row < 9; ++row){
				if(board[9*row + column]&digitMask[i]){
					if(first != -1){
						goto doneColumn;
					}
					first = 9*row + column;
				}
			}
			if(first != -1){
				if(board[first] != digitMask[i]){
					board[first] = digitMask[i];
					if(!eliminateNeighbors(board, first, left)){
						return 0;
					}
					--*left;
				}
			}
			doneColumn:;
		}
		for(int baseRow = 0; baseRow < 9; baseRow += 3){
			for(int baseColumn = 0; baseColumn < 9; baseColumn +=3){
				first = -1;
				for(int row = 0; row < 3; ++row){
					for(int column = 0; column < 3; ++column){
						if(board[9*(baseRow + row) + (baseColumn + column)]&digitMask[i]){
							if(first != -1){
								goto doneCell;
							}
							first = 9*(baseRow + row) + (baseColumn + column);
						}
					}
				}
				if(first != -1){
					if(board[first] != digitMask[i]){
						board[first] = digitMask[i];
						if(!eliminateNeighbors(board, first, left)){
							return 0;
						}
						--*left;
					}
				}
				doneCell:;
			}
		}
	}
	return 1;
}

int repeatedSelect(int *board, int *left){
	int prevLeft;
	do{
		prevLeft = *left;
		if(!selectLoneNumbers(board, left)){
			return 0;
		}
	}while(*left != prevLeft);
	return 1;
}

int *getClues(int *board){//puts number of clues as first element
	static int clues[61];//it's hardly a puzzle with more than 60 clues.
	int i = 0;
	for(int cell = 0; cell < 81; ++cell){
		if(board[cell] != 0b111111111){
			clues[++i] = cell;
		}
	}
	clues[0] = i;
	return clues;
}

int minimumPossibilitiesCell(int *board){
	int minp = 10, minc, p;
	for(int i = 0; i < 81; ++i){
		p = __builtin_popcount(board[i]);//oh the GNU extensions
		if(p == 1){
			continue;
		}
		if(p == 2){
			return i;
		}
		if(p < minp){
			minp = p;
			minc = i;
		}
	}
	return minc;
}

int backtrackingSolve(int *board, int *left){
	int thisBoard[81], thisLeft, pivot = minimumPossibilitiesCell(board);
	for(int i = 1; i <= 9; ++i){
		if(board[pivot]&digitMask[i]){
			memcpy(thisBoard, board, 9*9*sizeof(int));
			thisLeft = *left - 1;
			thisBoard[pivot] = digitMask[i];
			if(!eliminateNeighbors(thisBoard, pivot, &thisLeft)){
				continue;
			}
			if(!thisLeft){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
			if(!repeatedSelect(thisBoard, &thisLeft)){
				continue;
			}
			if(!thisLeft){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
			if(backtrackingSolve(thisBoard, &thisLeft)){
				memcpy(board, thisBoard, 9*9*sizeof(int));
				return 1;
			}
		}
	}
	return 0;
}

int solve(int *board){
	int *clues = getClues(board);
	int left = 81 - clues[0];
	for(int i = 1; i <= clues[0]; ++i){
		if(!eliminateNeighbors(board, clues[i], &left)){
			printf("eliminateNeighbors left some cell empty for the %ith hint\n", i);
			return 0;
		}
		if(!left){
			return 1;
		}
	}
	if(!repeatedSelect(board, &left)){
		puts("repeatedSelect left some cell empty");
		return 0;
	}
	if(!left){
		return 1;
	}
	return backtrackingSolve(board, &left);
}

int corner(int *board){
	return 100*digit(board[0]) + 10*digit(board[1]) + digit(board[2]);
}

int main(void){
	FILE *file = fopen("sudoku.txt", "r");
	if(!file){
		puts("could not open file");
		return 1;
	}
	int board[81], s = 0;
	for(int i = 0; i < 50; ++i){
		if(!readBoard(file, board)){
			puts("could not read board");
			return 1;
		}
		//puts("input----");
		//printBoard(board);
		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		if(!solve(board)){
			printf("could not solve board %i\n", i);
			return 1;
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		//puts("output---");
		printBoard(board);
		printf("solved in %.3fus\n", end.tv_sec*1e6 + end.tv_nsec*1e-3 - start.tv_sec*1e6 - start.tv_nsec*1e-3);
		s += corner(board);
	}
	fclose(file);
	printf("%i\n", s);
}

