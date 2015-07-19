#include <math.h>
#include <stdio.h>
inline bool p(unsigned long long n){
  return fmod(sqrt(1+24*n),6)==5;
}
inline bool h(unsigned long long n){
  return fmod(sqrt(1+8*n),4)==3;
}
int main(){
  for(unsigned long long i=1,t=1;;t+=++i){
    if(p(t)&&h(t)){
      printf("%ull\n",t);
      if(t>40755)break;
    }
  }
}
