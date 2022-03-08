#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <sys/resource.h>

/* For each rational p/q we try, we must try every d such that q | 59*d and d <= 1312.
 * That is, if 59 | q, d = k*q/59 for some k, otherwise d = k*q
 * Oh wow
 * This means q <= 59*1312 = 77408, and for sufficiently large but still smaller q we
 * have more nice properties.  For example, we know 77407 is not permissible because it
 * is not divisible by 59 and would force d to be too large.
 * If q is not divisible by 59, it can be at most 1312.
 * 
 * Let s = sum(l_i) and w = sum(d*l_i*B_i/A_i).
 * Then d*s*(2*3*41)/((5*59)*w) = p^2/q^2
 * 
 * So (2*3*41)*d*s = k*p^2 and (5*59)*w = k*q^2
 * 
 * p^2 | (2*3*41)*d*s
 * 
 * g = gcd(p^2, 2*3*41*d)
 * 
 * p^2/g*k = 2*3*41*d/g*s
 * 
 * k = 2*3*41*d/g*i
 * 
 * s = p^2/g*i
 * 
 * To isolate w, we also need k*q^2/5*59 to be an integer, so now let h = gcd(q^2, 5*59) and we have
 * 5*59/h*w = k*q^2/h
 * 
 * k = 5*59/h*j
 * 
 * so overall k = lcm(2*3*41*d/g, 5*59/h)*i for any integer i.
 * 
 * However, it would be ideal if we could compute f = lcm(2*3*41*d/g, 5*59/h) with only one gcd computation or something.
 * g = lcm(2*3*41*d/gcd(2*3*41*d, p^2), 5*59/gcd(5*59, q))
 * 
 * (2*3*41)*d*s = g*p^2*i and (5*59)*w = g*q^2*i
 * 
 * s = lcm(2*3*41*d/gcd(2*3*41*d, p^2), 5*59/gcd(5*59, q))*p^2/(2*3*41*d)
 * 
 * 
 * s = l1 + l2 + l3 + l4 + l5
 * w = d*(5/41*l1 + 59/41*l2 + 59/41*l3 + 59/90*l4 + 59/41*l5)
 * l2, l3, l5
 * 
 * l1 = s - l2 - l3 - l4 - l5
 * w = d*(5/41*s + 54/41*l2 + 54/41*l3 + (59/90 - 5/41)*l4 + 54/41*l5)
 * w - d*(5/41*s + 54/41*l2 + 54/41*l3 + 54/41*l5) = (59/90 - 5/41)*l4
 * 
 * l4 = (90*41/(59*41 + 5*90))*(w - d*(5/41*s + 54/41*l2 + 54/41*l3 + 54/41*l5))
 */

//static const uint64_t max_denom = 387040;
static const uint64_t max_denom = 77408;

static uint64_t visited = 0;

uint32_t gcd(uint32_t a, uint32_t b){
	while(1){
		a %= b;
		if(!a){
			return b;
		}
		b %= a;
		if(!b){
			return a;
		}
	}
}

static inline void visitM(uint64_t p, uint64_t q){
    uint64_t k = q, h = 59;
    if(q%59 == 0){
        k /= 59;
        h = 1;
    }
    if(q%5){//h here represents 5*59/gcd(5*59, q^2) = 5*59/gcd(5*59, q)
        h *= 5;
    }
    for(uint64_t d = k; d <= 1312; d += k){
        uint64_t g = 2*3*41*d/gcd(p*p, 2*3*41*d);//we might be able to simplify this
        if(g%5){
            g *= 5;
        }
        if(g%59){
            g *= 59;
        }
        //(2*3*41)*d*s = g*p^2*i and (5*59)*w = g*q^2*i
        uint64_t sk = g*p*p/(2*3*41*d);
        uint64_t wk = g*q*q/(5*59);//again we might be able to simplify this
        for(uint64_t s = sk, w = wk; s <= 18880ull/d && w <= 15744ull*q/p; s += sk, w += wk){
            
        }
    }
}

static void searchSternBrocotPreorder(uint64_t a, uint64_t b, uint64_t c, uint64_t d){
    uint64_t p = a + c;
    uint64_t q = b + d;
    if(q > max_denom){
        return;
    }
    int64_t left_ord = 1475ull*p - 1476ull*q;
    int64_t right_ord = 133909ull*q - 51209ull*p;
    if(left_ord >= 0 && right_ord >= 0){
        visitM(p, q);
        ++visited;
    }
    if(left_ord > 0){
        searchSternBrocotPreorder(a, b, p, q);
    }
    if(right_ord > 0){
        searchSternBrocotPreorder(p, q, c, d);
    }
}

int main(int argc, char **argv){
    double expected = (133909./51209. - 1476./1475.)*max_denom*(max_denom + 1)/2.*6./M_PI/M_PI;
    double expected_time = expected*6.548624e-09;
    printf("Expect to find ~ %f p/q in the region in %fs\n", expected, expected_time);
    {
        const rlim_t stack_size = 16*sizeof(uint64_t)*max_denom + 16*1024*1024;
        struct rlimit rl;
        int status = getrlimit(RLIMIT_STACK, &rl);
        if(status){
            fprintf(stderr, "\e[1;31mERROR: Could not get current stack size!\e[0m\n");
            exit(1);
        }
        if(rl.rlim_cur < stack_size){
            rl.rlim_cur = stack_size;
            status = setrlimit(RLIMIT_STACK, &rl);
            if(status){
                fprintf(stderr, "\e[1;31mERROR: Could not allocate enough stack space!\e[0m\n");
                exit(1);
            }
            printf("Successfully allocated stack space\n");
        }
    }
    searchSternBrocotPreorder(1, 1, 1, 0);
    printf("Found %"PRIu64" reduced p/q\n", visited);
}

