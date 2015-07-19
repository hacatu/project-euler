package main

// #cgo LDFLAGS: -L/usr/local/lib/ -Wl,-rpath=/usr/local/lib/ -lprimesieve
// #include <stdlib.h>
// #include <primesieve.h>
import "C"
import (
	"fmt"
	"math"
	"unsafe"
	"reflect"
)

const number = 600851475143

func main() {
	r := C.uint64_t(math.Sqrt(number))
	var s C.size_t
	pp := C.primesieve_generate_primes(0, r, &s, C.UINT64_PRIMES)
	hdr := reflect.SliceHeader{
		Data: uintptr(pp),
		Len: int(s),
		Cap: int(s),
	}
	ps := *(*[]C.uint64_t)(unsafe.Pointer(&hdr))//Forgive this criminal hackery
	
	n := C.uint64_t(number)
	
	for _, p := range ps {
		if p > r {
			break
		}
		if n%p == 0 {
			n /= p
			r = C.uint64_t(math.Sqrt(float64(n)))
		}
	}
	
	C.primesieve_free(unsafe.Pointer(pp))
	
	fmt.Println(n)
}

