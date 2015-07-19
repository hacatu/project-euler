#include <stdio.h>
#include "pes30.cxx"
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
  std::vector<big> P;
  sievePrimes(10000,P);
  int m=P.back(),a,b;
  for(int p:P){
    if(p<1488)continue;
    //for(int i=2;i<=(m-p)/2;i+=2){
      if(!isPrime(a=p+3330,P))continue;
      if(!isPrime(b=p+6660,P))continue;
      //printf("%i+%i\n",p,i);
      if(!rp(p,a))continue;
      if(!rp(p,b))continue;
      printf("%i,%i,%i\n",p,a,b);
      break;
    //}
  }
}
