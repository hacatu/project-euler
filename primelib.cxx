#include <math.h>
#include <vector>

std::vector<int> P={3};

void addPrimes(int);

bool isPrime(int n){//returns true for evens; pre-check
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
