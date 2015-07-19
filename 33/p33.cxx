#include <iostream>
int main(){
  int f[]={1,1,2,6,24,120,720,5040,40320,362880},t=0,n,s;
  for(int i=10;i<1000000;++i){
    s=0;n=i;
    while(n){
      s+=f[n%10];
      n/=10;
    }
    if(s==i)t+=i;
    s=0;
  }
  std::cout<<t;
}
