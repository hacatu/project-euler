#include <stdio.h>
#include <cstdint>
#include <vector>
#include "pes30.cxx"
using namespace std;
//highest prime under 1000000 is 999983
/*
vector<uint64_t> P=sievePrimes(1000000);
for(uint64_t sum=0, i=0; i<P.size(); ++i){
	sum+=P[i];
	if(sum>999983){
		sum-=P[i];
		printf("%li is the sum of the %li primes up to %li\n",sum,i,P[i-1]);
		break;
	}
}
*/
//997661 is the sum of the 546 primes up to 3931//should be 545
//there must be at least 22 primes in the sum
/*
vector<uint64_t> P=sievePrimes(1000000);
uint64_t sum=0;
for(uint64_t i=0; i<22; ++i){
	sum+=P[i];
}
for(uint64_t i=0; i+22<P.size(); ++i){
	sum+=P[i+22]-P[i];
	if(sum>999983){
		sum-=P[i+22]-P[i-1];
		printf("%li is the sum of the 22 primes from %li to %li\n",sum,P[i-1],P[i+21]);
		break;
	}
}
*/
//999830 is the sum of the 22 primes from 45319 to 45569
/*printf("%lu\n",(sievePrimes(45569)).size());
*/
//we only need to look at primes up to 45569 (4724 primes)
//we need an odd number of primes or to start at 2
int main(){
	uint64_t sum,l,i;
	vector<uint64_t> P=sievePrimes(1000000);
	for(l=545;l>20;l-=1){
		sum=0;
		//printf("Trying a length of %lu\n",l);
		if(l&1){
			for(i=1; i<=l; ++i){
				sum+=P[i];
			}
			//printf("The sum is %lu\n",sum);
			if(isPrime(sum,P)){
				i=1;
				goto PRINT_SUM;
			}
			for(i=1; i<P.size()-l; ++i){
				sum+=P[i+l]-P[i];
				//printf("    The sum is %lu\n",sum);
				if(sum>=1000000)
					break;
				if(isPrime(sum,P)){
					++i;
					goto PRINT_SUM;
				}
			}
		}else{
			for(i=0; i<l; ++i){
				sum+=P[i];
			}
			//printf("The sum is %lu\n",sum);
			if(isPrime(sum,P)){
				i=0;
				goto PRINT_SUM;
			}
		}
	}
	PRINT_SUM:
	printf("I found %lu is the sum of %lu primes:\n",sum,l);
	//for(uint64_t j=i;j<i+l;++j)printf("%lu\n",P[j]);
}

