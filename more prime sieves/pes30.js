var offsets=[6,4,2,4,2,4,6,2],
    bool_offsets=[
9, 0, 9, 9, 9, 9,
9, 1, 9, 9, 9, 2,
9, 3, 9, 9, 9, 4,
9, 5, 9, 9, 9, 6,
9, 9, 9, 9, 9, 7
];
function sievePrimes(max){
    var multiple,primes,n=[];
    for(var i=0; i < max/15*4+8; ++i) n[i]=true;
    primes=[2,3,5];
    for(var i=1, p=7, step=1; p <= max; ++i, p += offsets[step], ++step == 8 ? step=0:0){
        if(!n[i])
            continue;
        primes.push(p);
        for(var j=i, p2=p, step2=step; j <= max/p; ++j, p2 += offsets[step2], ++step2 == 8 ? step2=0:0){
            if(!n[j])
                continue;
            multiple=p*p2;
            do{
                n[~~(multiple/30)*8 + bool_offsets[ multiple%30 ]]=false;
            }while((multiple*=p) <= max);
        }
    }
    return primes;
}
alert(sievePrimes(100));
