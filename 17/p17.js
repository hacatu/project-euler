var t;
function i2s(i){
    if(!i)return"";
    else if(i<13)
        return[
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            "ten",
            "eleven",
            "twelve"
        ][i-1];
    else if(i<16)
        return[
            "thir",
            "four",
            "fif"
        ][i-13]+"teen";
    else if(i<20)
        return i2s(i%10)+"teen";
    else if(i<60){
        t=i2s(i%10);
        if(t)t="-"+t;
        return[
            "twen",
            "thir",
            "for",
            "fif"
        ][~~(i/10)-2]+"ty"+t;
    }else if(i<100){
        t=i2s(i%10);
        if(t)t="-"+t;
        return i2s(~~(i/10))+"ty"+t;
    }else if(i<1000){
        t=i2s(i%100);
        if(t)t=" and "+t;
        return i2s(~~(i/100))+" hundred"+t;
    }else if(i==1000)return"one thousand";
    else return NaN;
}
alert(i2s(prompt("ASDF:")));
