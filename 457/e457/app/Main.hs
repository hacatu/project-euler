module Main where

import Math.NumberTheory.Moduli.Sqrt
import Math.NumberTheory.Primes
import Data.Maybe

minRoot p = case sqrtsModPrimePower 13 p 2 of
    [] -> 0
    p -> foldr1 min p

main = print $ sum $ map minRoot $ takeWhile ((<= 10000000) . unPrime) $ enumFrom $ fromJust $ isPrime 3

