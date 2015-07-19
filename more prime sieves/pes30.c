//""split(" ").map(function(s){return s+" is love, "+s+" is life"}).join("\n")
/* pes30.c
 * prime number generator for Project Euler
 * Print all the primes up to 2x10^9.
 * 2014-05-08: started by hacatu
 * implementation of Euler's sieve (a modified Sieve of Eratosthenes)
 * http://en.wikipedia.org/wiki/Euler%27s_sieve
 * using wheel factorization with a wheel of size 30 to reduce memory requirements
 * http://en.wikipedia.org/wiki/wheel_factorization
 */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

const int offsets[]={6,4,2,4,2,4,6,2};//steps in the wheel 1,7,11,13,17,19,23,29,1...

const int bool_offsets[]={//used to find a number's spot on the wheel, for accessing its primality in the vector
9, 0, 9, 9, 9, 9,//the nines should never be accessed
9, 1, 9, 9, 9, 2,
9, 3, 9, 9, 9, 4,
9, 5, 9, 9, 9, 6,
9, 9, 9, 9, 9, 7
};

//create a vector with all primes under some value:
uint64_t* sievePrimes(uint64_t max){
    uint64_t* is_p=calloc(max/30*8+8, sizeof(char));//array of whether every number relatively prime to 30 is still a candidate prime.
    //8 biooleans are needed for every 30 numbers (since 8 are relatively prime with 30), so max/30*8, plus 8 because max/30 rounds down.
    if(!is_p) return 0;
    uint64_t primes_s=max/log(max);
    uint64_t* primes=malloc(primes_s*sizeof(uint64_t));
    if(!primes) return 0;
    primes[0]=2; primes[1]=3; primes[2]=5;//because the sieve skips all multiples of 2,3, and 5, start with them.
    uint64_t multiple;
    //for every number marked prime less than max, mark its multiples with
    //every number still marked prime over it as composite.
    uint64_t r=sqrt(max);
    for(uint64_t i=1, p=7, step=1; p <= max; ++i, p += offsets[step], ++step == 8 ? step=0:0){
        if(!is_p[i])//if p is not prime (using i for the index holding the primality of p, to avoid computing the index for every number)
            continue;
        primes.push_back(p);//add p to the list of primes
        //finds every multiple of i and a (still marked) prime greater than i
        for(uint64_t j=i, p2=p, step2=step; p2 <= max/p; ++j, p2 += offsets[step2], ++step2 == 8 ? step2=0:0){
            if(!is_p[j])//skip nonprimes
                continue;
            multiple=p*p2;
            do{
                is_p[multiple/30*8 + bool_offsets[ multiple%30 ]]=false;
            }while((multiple*=p) <= max);
        }
    }
    return primes;
}

