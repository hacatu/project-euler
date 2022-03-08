#include <stdio.h>

/* Riffle Shuffles: to riffle shuffle 2n cards [0, 2n),
 * we take the cards [0, n) and the cards [n, 2n) and recombine
 * them alternatingly, starting with 0 from [0, n) and ending
 * with 2n from [n, 2n).
 * Consider this for 2n = 4: the shuffle has a permutation matrix,
 * so if we want to know where in the output some card i from the
 * input ends up in we can use the permutation matrix.
 * 
 * for all i in [0, 2n): there exists a unique o in [0, 2n) so that:
 * e_o = P * e_i, namely o = { 2*i           if i in [0, n)
 *                           { 2*(i - n) + 1 otherwise
 * 
 * P = [e_o(0)] = [1      ]
 *     [e_o(1)] = [    1  ]
 *     [e_o(2)] = [  1    ]
 *     [e_o(3)] = [      1]
 * 
 * and s(4) = 2.  Note that clearly s(2) = 1
 * 
 * For 2n = 6:
 * P = [1          ]
 *     [    1      ]
 *     [        1  ]
 *     [  1        ]
 *     [      1    ]
 *     [          1]
 * 
 * and s(6) = 4.
 * 
 * Note that only the [1, 2n - 1) entries of [0, 2n) matter since the
 * first and last entries always get mapped to themselves.
 * So we can look at the riffle shuffles on 2n cards as permutation
 * matrices on 2n - 2 elements.  These are orthogonal matrices, and
 * the part with the fixed cards chopped off is trace 0 and determinant
 * -1.  
 */

int main(){
	
}

