#include <stdio.h>
#include <vector>
#include "primelib.cxx"
int main(){
  addPrimes(1000000);
  int l,t,c=1;
  bool a;
  for(int p:P){
    if(p>99999){l=5;t=100000;}
    else if(p>9999){l=4;t=10000;}
    else if(p>999){l=3;t=1000;}
    else if(p>99){l=2;t=100;}
    else if(p>9){l=1;t=10;}
    else{
      ++c;
      continue;
    }
    a=true;
    for(;a&&l;--l){
      p=p/10+p%10*t;
      if((p%2==0)||!isPrime(p))a=false;
    }
    if(a)++c;
  }
  printf("c=%i\n",c);
}

