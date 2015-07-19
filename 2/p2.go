/* p2.go
 * by hacatu
 * 
 * This program solves Project Euler problem 2, to find the sum of the even Fibonacci numbers under 4000000.
 * The Fibbonacci number F[k + 2] = F[k + 1] + F[k].  This leads to the linear transformation matrix ((1, 1), (1, 0))
 * (F[k + 2], F[k + 1]) = ((1, 1), (1, 0))*(F[k + 1], F[k]).  Every third Fibonacci number is even, so cube the matrix
 * and get ((3, 2), (2, 1))
 * (F[k + 4], F[k + 3]) = ((3, 2), (2, 1))*(F[k + 1], F[k]).  To parallelize this, raise the matrix to the power of the number
 * of cores and start each core at an offset from (F[k + 1], F[k]) = (2, 1) by applying the ((3, 2), (2, 1)) transformation
 * repeatedly.
 * (F[k + 3*n + 1], F[k + 3*n]) = ((3, 2), (2, 1))^n*(F[k + 1], F[k]).  See https://en.wikipedia.org/wiki/Fibonacci_number.
 */
package main

import (
	"fmt"
	"runtime"
	"github.com/gonum/matrix/mat64"
)

func round(f float64) int {
	return int(f + 0.5)
}

/* Calculates the appropriate power of the next-even transformation matrix
 * and returns the coefficients in the linear equation it represents.
 */
func calcTMat(n int) (aa, ab, ba, bb int) {
	t1Mat := mat64.NewDense(2, 2, []float64{3, 2, 2, 1})
	tMat := new(mat64.Dense)
	tMat.Pow(t1Mat, n);
	
	return round(tMat.At(0, 0)),
	       round(tMat.At(0, 1)),
	       round(tMat.At(1, 0)),
	       round(tMat.At(1, 1))
}

func main() {
	cpus := runtime.NumCPU()
	fmt.Printf("Running on %d cores.\n", cpus)
	
	sums := make(chan int, cpus)
	
	aa, ab, ba, bb := calcTMat(cpus)
	
	fmt.Printf("Using transformation a, b = %d*a + %d*b, %d*a + %d*b.\n", aa, ab, ba, bb)
	
	for a, b, i := 2, 1, 0; i < cpus; a, b, i = 3*a + 2*b, 2*a + b, i + 1 {
		go func(a, b int){
			sum := 0
			for a < 4000000 {
				sum += a
				a, b = aa*a + ab*b, ba*a + bb*b
			}
			sums <- sum
		}(a, b)
	}
	
	sum := 0
	for i := 0; i < cpus; i++ {
		sum += <-sums
	}
	
	fmt.Println("The sum of the even Fibonacci number under 4000000 is", sum);
}

