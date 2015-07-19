#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
int d(int n){
	int prod=1,p,
		r=sqrt(n);
	for(int k=2;k<=r;k++){
		p=1;
		while(n%k==0){
			p=p*k+1;
			n/=k;
		}
	prod*=p;
	}
	if(n>1)prod*=1+n;
	return prod;
}
int main(){
	int s=0;
	vector<int> a;
	bool S[28123*2+1]={false};
	for(int i=1;i<20150;i++)if(d(i)>>1>i)a.push_back(i);
	for(int i=0;i<a.size();i++)
    for(int j=0;j<=i;j++)S[a[i]+a[j]]=true;
	for(int i=1;i<20162;i++)if(!S[i])s+=i;
	cout<<s;
}
