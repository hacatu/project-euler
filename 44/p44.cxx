#include <math.h>
#include <stdio.h>
#include <unordered_set>
inline unsigned int p(unsigned int n){
  return n*(3*n-1)/2;
}
int main(){
  std::unordered_set<unsigned int> isp;
  unsigned int P[53510];
  unsigned int pk;
  for(unsigned int d=1,pd=p(1);d<=53510;pd=p(++d)){
    isp.insert(pd);
    P[d]=pd;
  }
  for(unsigned int d=1,pd=P[1];;pd=P[++d]){
    for(unsigned int j=2,pjl=P[1],pj=P[2];pj-pjl<pd;pjl=pj,pj=P[++j]){
      if(isp.count(pk=pj+pd)==0)continue;
      if(isp.count(pj+pk)==0)continue;
      printf("%ui\n",pd);
      return 0;
    }
  }
}
