#include <iostream>
#include <vector>
using namespace std;
vector<int>N{0,1,32,243,1024,3125,7776,16807,32768,59049};
int q(int x){
	return N.size()<x?N[x%10]+q(x/10):N[x];
}
int main(){
	vector<int>M;
	int s=0;
	for(int i=10;i<1000000;++i)if(i==q(i))M.push_back(i);
	for(int i=0;i<M.size();++i)s+=M[i];
	cout<<s;
}
