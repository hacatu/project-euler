package main

import (
	"fmt"
	"os"
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

func main() {
	var (
		iss [max - min + 1]big.Int
		rss [max - min + 1]big.Int
		tmp [max - min + 1]big.Int
		scd [max - min + 1]bool
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
	
	rss[max - min].Set(&iss[max - min]);
	for i := int64(max - 1); i >= min; i-- {
		rss[i - min].Add(&iss[i - min], &rss[i + 1 - min])
	}
	
	sum.Div(&denom, big.NewInt(2))
	
	if !firstGreedy(sum, iss[:], rss[:], tmp[:], scd[:]) {
		fmt.Println("No solution")
		os.Exit(1)
	}
	
	for i := int64(min); i <= max; i++ {
		if scd[i - min] {
			fmt.Printf("%d,", i)
		}
	}
}

