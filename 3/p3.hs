import Data.Numbers.Primes
last $ filter (\n->600851475143`mod`n==0) $ takeWhile (<775147) primes
