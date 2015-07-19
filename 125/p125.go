package main

import (
	"fmt"
)

const MAX = 100000000

var reverse [1000]uint64

func init() {
	for n := uint64(0); n < 10; n++ {
		reverse[n] = n
	}
	for n := uint64(10); n < 100; n++ {
		reverse[n] = n%10*10 + n/10
	}
	for n := uint64(100); n < 1000; n++ {
		reverse[n] = n%10*99 + n%100 + n/100
	}
}

func isPalindrome(n uint64) bool {
	if n < 10 {
		return true
	}
	if n%10 == 0 {
		return false
	}
	if n < 100 {
		return n%11 == 0
	}
	if n < 1000 {
		return n%10 == n/100
	}
	if n < 10000 {
		return n%100 == reverse[n/100]
	}
	if n < 100000 {
		return n%100 == reverse[n/1000]
	}
	if n < 1000000 {
		return n%1000 == reverse[n/1000]
	}
	if n < 10000000 {
		return n%1000 == reverse[n/10000]
	}
	if n%1000 != reverse[n/100000] {
		return false
	}
	return n/1000%100%11 == 0
}

func main() {
	init()
	S := uint64(0)
	
}

