#include <stdio.h>
typedef unsigned long long bignum;
typedef unsigned char barr;
const unsigned char setms[]={1,2,4,8,16,32,64,128},
                    clearms[]={254,253,251,247,239,223,191,127};
inline void setbit(bignum n,barr l[]){
  l[n>>3]|=setms[n&7];
}
inline void clearbit(bignum n,barr l[]){
  l[n>>3]&=clearms[n&7];
}
inline barr getbit(bignum n,barr l[]){
  return l[n>>3]&setms[n&7];
}
void sieve(bignum n){
  barr l[(n>>3)+1];
  for(bignum i=0;i<=(n>>3)+1;++i)l[i]=255;
  clearbit(0,l);
  clearbit(1,l);
  bignum f=2;
  bignum c;
  while(f*f<=n){
    c=2*f;
    while(c<=n){
      clearbit(c,l);
      c+=f;
    }
    do ++f;
    while(!getbit(f,l));
  }
  for(bignum i=2;i<=(n>>3)+1;++i)getbit(i,l)&&printf("%ull\n",i);
}
int main(){
  barr test[]={255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};
  getbit(1,test)&&printf("test[1]\n");
  for(int i=0;i<8;++i){clearbit(9*i,test);printf("%i i%i\n",test[i],i);}
  //sieve(30);
}
