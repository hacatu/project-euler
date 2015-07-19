#include <stdio.h>
#include "primelib.cxx"
inline int s(int n,int t){
  return n/t/10*t+n%t;
}
inline bool rp(int a,int b){
  if(a==b)return true;
  int o=a%10,c=b,t=1;
  a/=10;
  while(c){
    if((c%10)==o)return rp(a,s(b,t));
    c/=10;
    t*=10;
  }
  return false;
}
int main(){
  addPrimes(10000);
  /*int m=P.back(),a,b;
  for(int p:P){
    if(p<1488)continue;
    //for(int i=2;i<=(m-p)/2;i+=2){
      if(!isPrime(a=p+3330))continue;
      if(!isPrime(b=p+6660))continue;
      //printf("%i+%i\n",p,i);
      if(!rp(p,a))continue;
      if(!rp(p,b))continue;
      printf("%i,%i,%i\n",p,a,b);
      break;
    //}
  }*/
}
