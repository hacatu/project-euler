#include <stdio.h>
int main(){
  int m,v,a,b,c,d,e,f,g,h;
  for(int i=9438;i>9182;--i){
    f=i%10;
    if(f==1||f==9||f%5==0)continue;
    h=i;
    h/=10;
    g=h%10;
    if(g==1||g==0||g==9||g==f)continue;
    h/=10;
    h%=10;
    if(h==1||h==0||h==9||h==g||h==f)continue;
    v=100002*i;
    printf("%i\n",v);
    e=v;
    a=e%10;
    e/=10;
    b=e%10;
    if(b==1||b==0||b==9||b==f||b==g||b==h||b==a)continue;
    e/=10;
    c=e%10;
    if(c==1||c==0||c==9||c==f||c==g||c==h||c==b||c==a)continue;
    e/=10;
    d=e%10;
    if(d==1||d==0||d==9||d==f||d==g||d==h||d==c||d==b||d==a)continue;
    printf("\n%i\n",v);
    break;
  }
}
