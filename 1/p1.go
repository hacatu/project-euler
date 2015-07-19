package main

import "fmt"
import "runtime"

func main() {
	cpus := runtime.NumCPU()
	fmt.Printf("Running on %d cores.\n", cpus)
	
	sums := make(chan int, cpus)
	
	sum := 0
	
	for s := 0; s < 1000; s += 1000/cpus {
		go func(s int) {
			sum := 0
			for i := s; i < s + 1000/cpus; i++ {
				if i%3 == 0 || i%5 == 0 {
					sum += i
				}
			}
			sums <- sum
		}(s)
	}
	
	for i := 0; i < cpus; i++ {
		sum += <-sums
	}
	
	fmt.Println("The sum of all multiples of 3 or 5 below 1000 is", sum)
}

