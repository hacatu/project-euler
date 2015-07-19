#include <stdio.h>
bool isP2(int n){
  int i=1;
  while(n>=i)i<<=1;
  i>>=1;
  for(int j=1;j<=i;j<<=1,i>>=1)
    if(!(n&i)!=!(n&j))
      return false;
  return true;
}
int main(){
  int s=0,n,l,a;
  for(int i=1;i<1000;i+=2){
    n=i;a=0;l=1;
    while(n){
      l*=10;
      a=10*a+n%10;
      n/=10;
    }
    n=a*l+i;
    if(isP2(n))s+=n;
    if(i<10){
      if(isP2(n=i*101))s+=n;
      if(isP2(n=i*1001))s+=n;
      if(isP2(n=i*10001))s+=n;
      if(isP2(n=i*100001))s+=n;
    }else if(i<100){
      if(isP2(n=a*l*10+i))s+=n;
      if(isP2(n=a*l*100+i))s+=n;
    }
    n=i;a=0;l=10;
    while(n>9){
      l*=10;
      a=10*a+n%10;
      n/=10;
    }
    n=a*l+i;
    if(isP2(n))s+=n;
  }
  printf("\n%i\n",s);
}
