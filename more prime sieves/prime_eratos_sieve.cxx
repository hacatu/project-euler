#include <stdio.h>
typedef unsigned long long bignum;
void sieve(bignum n){
  char l[n+1];
  for(bignum i=2;i<=n;++i)l[i]=1;
  l[0]=0;
  l[1]=0;
  bignum f=2;
  bignum c;
  while(f*f<=n){
    c=2*f;
    while(c<=n){
      l[c]=0;
      c+=f;
    }
    do ++f;
    while(!l[f]);
  }
//  for(bignum i=2;i<=n;++i)if(l[i])printf("%ull\n",i);
}
int main(){
  sieve(1000000);
}
