#include <math.h>
#include <stdio.h>
int main(){
  int p[2000];
  for(int i=0;i<2000;++i)p[i]=0;
  int c,m=0,n=0;
  for(int a=1;a<999;++a){
    for(int b=1;(b<=a)&&(b<1001-2*a);++b){
      if((c=sqrt(double(a*a+b*b)))%1==0)++p[a+b+c];
    }
  }
  for(int i=0;i<1001;++i){
    if(p[i]>m){
      m=p[i];
      n=i;
    }
  }
  printf("%i %i\n",n,m);
}

