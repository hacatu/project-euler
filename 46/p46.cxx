#include <stdio.h>
#include <math.h>
#include "primelib.cxx"
int main(){
  int r;
  bool done;
  for(int i=33;;i+=2){
    if(isPrime(i))continue;
    r=sqrt(i/2);
    done=true;
    for(int j=1;j<=r;++j){
      if(isPrime(i-2*j*j))done=false;
    }
    if(done){
      printf("%i\n",i);
      break;
    }
  }
}
