#include <stdio.h>
#include <stdlib.h>

#define N 6
#define START_0 11
#define START_SUM 87
#define MAX_SUM 117
#define MAX_SETSTR 111720222324

int set[N] = {START_0};
int sums[MAX_SUM + 1] = {[0] = 1, [START_0] = 1};
int ss[2<<N] = {0, START_0};//subset sums

//0,1,01,11,001,101,011,111
int sum(int n){//0->0, 1->1, 2->3, ... n->(1<<n) - 1
	return ss[(2<<n) - 1];
}

int rSum(int n){//remaining sum
	return (N - n)*set[n] + (N - 1 - n)*(N - n)/2;//number of remaining spots times current spot plus spots left minus one th triangular number
}

int rSumFits(int n, int m){
	return rSum(n) <= m;
}

int sumAsc(int n){//s0 + s1 > s(N - 1), s0 + s1 + s2 > s(N - 2), ..., s0 + s1 + ... + s(a) > s(N - a); n = (N - a) --> a = N - n
	if((N - n) >= n){
		return 1;
	}
	return sum(N - n) > set[n];//error
}

int addSums(int n){//the sums on [0, 1<<n) will exist, and we will add [1<<n, 2<<n)
	int t;
	for(int i = 0; i < (1<<n); ++i){//for every index in the sums that exist
		t = ss[i] + set[n];//find that sum plus the current element
		if(sums[t]){//this sum has been created already; reset the sums
			i += (1<<n);
			for(; i >= (1<<n); --i){
				sums[ss[i]] = 0;
				ss[i] = 0;
			}//don't reset the subset sums (ss); they can just get overwritten
			return 0;
		}//otherwise add this new sum
		ss[i + (1<<n)] = t;
		sums[t] = 1;
	}
	return 1;
}

void clearSums(int n){//the sums on [0, 1<<n) should exist, but remove [1<<n, 2<<n)
	for(int i = (1<<n); i < (2<<n); ++i){
		sums[ss[i]] = 0;
		ss[i] = 0;
	}//don't reset ss; it can just get overwritten
}

void printSetStr(void){
	for(int i = 0; i < N; ++i){
		printf("%i", set[i]);
	}
	puts("");
}

void printSS(void){
	printf("%i", ss[0]);
	for(int i = 1; ss[i]; ++i){
		printf(",%i", ss[i]);
	}
	puts("");
}

int fill(int n, int m){
	if(n == N - 1){
		set[n] = m;
		if(!sumAsc(n)){
			return 0;
		}
		if(!addSums(n)){
			return 0;
		}
		return 1;
	}
	for(set[n] = set[n - 1] + 1; rSumFits(n, m) && sumAsc(n); ++set[n]){/*
		if(set[1] == 18){
			printf("set[%i]=%i\n", n, set[n]);
		}*/
		if(!addSums(n)){
			continue;
		}/*
		if(set[1] == 18){
			puts("added sums");
			printSS();
		}*/
		if(fill(n + 1, m - set[n])){
			return 1;
		}
		clearSums(n);
	}
	return 0;
}

int main(void){
	int m = 115;
	for(int m = START_SUM; m < MAX_SUM; ++m){
		if(fill(1, m - set[0])){
			break;
		}
	}
	printSetStr();
}

