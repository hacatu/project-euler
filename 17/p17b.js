var t;
function i2s(i){
    if(!i)return 0;
    else if(i<13)return[3,3,5,4,4,3,5,5,4,3,6,6][i-1];
    else if(i==18)return 8;
    else if(i<16)return[4,4,3][i-13]+4;
    else if(i<20)return i2s(i%10)+4;
    else if(~~(i/10)==8)return 6+i2s(i%10);
    else if(i<60)return[4,4,3,3][~~(i/10)-2]+2+i2s(i%10);
    else if(i<100)return i2s(~~(i/10))+2+i2s(i%10);
    else if(i<1000){
        t=i2s(i%100);
        if(t)t+=3;
        return i2s(~~(i/100))+7+t;
    }
}
for(var i=1,e=11;i<1000;i++)e+=i2s(i);
