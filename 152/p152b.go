package main

import (
	"fmt"
	"os"
	"runtime"
	"math/big"
)

const (
	min = 2
	max = 45
)

func flcm(a *big.Int, b *big.Int) {
	gcd := big.NewInt(0)
	gcd.GCD(nil, nil, a, b)
	a.Mul(a, b)
	a.Div(a, gcd)
}

/*
func firstGreedy(sum big.Int, iss []big.Int, rss []big.Int, tmp []big.Int, scd []bool) bool {
	if sum.Cmp(big.NewInt(0)) == 0 {
		return true
	}
	
	for i := 0; i < len(iss); i++ {
		if sum.Cmp(&iss[i]) >= 0 {
			if sum.Cmp(&rss[i]) > 0 {
				return false
			}
			
			tmp[0].Sub(&sum, &iss[i])
			scd[i] = true
			
			if firstGreedy(tmp[0], iss[i + 1:], rss[i + 1:], tmp[i + 1:], scd[i + 1:]) {
				return true
			}
			
			scd[i] = false
		}
	}
	
	return false
}
*/

func countSolutionsC(sum big.Int, parts []map[string]int) int {
	var (
		partc chan int
		sums []big.Int
		i int
		ret int
	)
	
	partc = make(chan int, len(parts[0]))
	sums = make([]big.Int, len(parts[0]))
	
	for k, v := range parts[0] {
		sums[i].Sub(&sum, k)
		go countSolutionsH(sums[i], parts[1:], v, partc)
		i++
	}
	
	for i := 0; i < len(parts[0]); i++ {
		ret += <- partc
	}
	
	return ret
}

func recSums(t big.Int, sum big.Int, part []big.Int, out chan<- big.Int) {
	if t.Cmp(&sum) > 0 {
		return
	}
	out <- sum
	if len(part) == 0 {
		return
	}
	recSums(t, sum, part[1:], out)
	var sumc big.Int
	sumc.Add(&sum, &part[0])
	recSums(t, sum, part[1:], out)
	
}

func recSumsW(t big.Int, sum big.Int, part []big.Int, out chan<- big.Int) {
	recSums(t, sum, part, out)
	close(out)
}

func findSums(sum big.Int, part []big.Int, out chan<- map[string]int) {
	var (
		spart map[string]int
		sumc chan big.Int
		s big.Int
	)
	
	spart = make(map[string]int)
	sumc = make(chan big.Int, 1<<uint(len(part)))
	s.Set(big.NewInt(0))
	go recSumsW(sum, s, part, sumc)
	
	for s := range sumc {
		spart[string(s.Bytes())]++
	}
	
	out <- spart
}

func countSolutions(sum big.Int, iss []big.Int) int {
	var (
		parts [][]big.Int
		cpus int
		partc chan map[string]int
		spart []map[string]int
	)
	
	cpus = runtime.NumCPU()
	parts = make([][]big.Int, cpus)
	partc = make(chan map[string]int, cpus)
	spart = make([]map[string]int, cpus)
	for i := 0; i < cpus; i++ {
		if i <= len(iss)%cpus {
			parts[i] = make([]big.Int, len(iss)/cpus + 1)
		} else {
			parts[i] = make([]big.Int, len(iss)/cpus)
		}
	}
	
	for i, e := range iss {
		parts[i%8][i/8] = e
	}
	
	for i := 0; i < cpus; i++ {
		go findSums(sum, parts[i], partc)
	}
	
	for i := 0; i < cpus; i++ {
		spart[i] = <- partc
	}
	
	return countSolutionsC(sum, spart)
}

func main() {
	var (
		iss [max - min + 1]big.Int
		sum big.Int
		denom big.Int
	)
	
	denom.Set(big.NewInt(1))
	
	for i := int64(min); i <= max; i++ {
		iss[i - min].Set(big.NewInt(i*i))
		flcm(&denom, &iss[i - min])
	}
	
	for i := int64(min); i <= max; i++ {
		iss[i - min].Div(&denom, &iss[i - min])
	}
	
	sum.Div(&denom, big.NewInt(2))
	sum.Neg(&sum)
	for i := int64(min); i <= max; i++ {
		sum.Add(&sum, &iss[i - min])
	}
	
	fmt.Println(countSolutions(sum, iss[:]))
	os.Exit(0)
}

