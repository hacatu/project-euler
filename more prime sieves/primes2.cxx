#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

vector<int> P={3};

void addPrimes(int);

bool isPrime(int n){//Won't work on even ints since only odds are tested
	int r=sqrt(n);
	if(r>P.back())addPrimes(r);
    for(int p:P){//replace with binary search
	    if(p>r)return true;
	    if(!(n%p))return false;
    }
    return true;
}

void addPrimes(int n){
    for(int i=P.back()+2;i<=n;i+=2){
	    if(isPrime(i))P.push_back(i);
    }
}

int main(){
    addPrimes(1000);
    for(int p:P)printf("%i\n",p);
}
