#include <stdio.h>
#include <math.h>
#include "primelib.cxx"
inline int count(int n){
  int f=0;
  if((n&1)==0){
    f=1;
    do{n>>=1;}while((n&1)==0);
  }
  if(n==1)return f;
  if(isPrime(n))return f+1;
  int r=sqrt(n);
  for(int p:P){
    if(p>r)break;
    if((n%p)==0){
      ++f;
      do{n/=p;}while((n%p)==0);
      r=sqrt(n);
    }
  }
  if(n==1)return f;
  return f+1;
}
int main(){
  int c;
  for(int i=210;;++i){
    c=0;
    while(count(i)>3){
      ++c;
      if(c==4){
        printf("%i\n",i-3);
        return 0;
      }
      ++i;
    }
  }
}
