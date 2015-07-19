//ptest.cxx
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <stdio.h>
#include "pes30.cxx"
using namespace std;
int main(int argc, char** argv){
	uint64_t max=1000000;
	bool print=true;
	if(argc>1){
		max=uint64_t(atoll(argv[1]));
	}
	if(argc>2){
		print=atoi(argv[2]);
	}
	vector<uint64_t> P=sievePrimes(max);
	if(print){
		for(uint64_t p : P){
			printf("%li\n",p);
		}
	}
}
