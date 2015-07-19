#include <stdio.h>
#include "primelib.cxx"
int main(){
  addPrimes(1000000);
/*  int n,s=0,c=0,i;
  bool prime[]={false,false,true,true,false,true,false,true,false,false};
  for(int p:P){
    if(p<10)goto end_of_loop;
    if(!prime[p%10])goto end_of_loop;
    n=p/10;
    while(n>9){
      if(!(n&1))goto end_of_loop;
      if(n%10==5)goto end_of_loop;
      n/=10;
    }
    if(!prime[n])goto end_of_loop;
    n=p/10;
    i=10;
    while(n>9){
      if(!isPrime(n))goto end_of_loop;
      n/=10;
      i*=10;
    }
    n=p;
    while(i>1){
      if(!isPrime(n-=n/i*i))goto end_of_loop;
      i/=10;
    }
    s+=p;
    if(++c==11)break;
    end_of_loop:;
  }
  printf("%i\n",s);
*/}
