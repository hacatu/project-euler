{-# LANGUAGE TemplateHaskell #-}

import Test.QuickCheck
import Data.Fixed

g :: (Double, Double, Double, Double, Double) -> Bool
g q = gk q || gr q where
	gk (a, b, c, d, e) = d + e < 90 &&
		(c < a || (c == a && (d < b || (d == b && e <= d))))
	gr q@(a, b, c, d, e) = d + e == 90 &&
		(um q ||
			(a - d == b - a && a - d > c - b && c - b >= d - c) ||
			(a - d == c - b && a - d > b - a && b - a >= d - c) ||
			(a - d == b - a && a - d == d - c && a - d >= c - b))
		where um (a, b, c, d, e) =
			a - d == maximum [a - d, b - a, c - b, d - c] &&
			(a + d > 90 || (a + d == 90 && b + c >= 90))

r :: (Double, Double, Double, Double, Double) -> Double -> (Double, Double, Double, Double, Double)
r q 0 = q
r q n = r (r' q) (n - 1) where
	r' (a, b, c, d, e) = (b, c, d, a, 180 - a - d - e)

m :: (Double, Double, Double, Double, Double) -> Double -> (Double, Double, Double, Double, Double)
m q 0 = q
m (a, b, c, d, e) 1 = (d + e - b, 180 - a - d - e, e, 180 - c - d - e, c)

t :: Double -> Double -> (Double, Double, Double, Double, Double) -> (Double, Double, Double, Double, Double)
t i j q = m (r q j) i

a --> b = (not a) || (a && b)

prop_Gi :: (Int, Int, Int, Int) -> Bool
prop_Gi f = (validAngles q) -->
	(all id [(g q) && (g $ t i j q) --> (q == t i j q) | i <- [0, 1], j <- [0, 3]])
	where q = c f

c :: (Int, Int, Int, Int) -> (Double, Double, Double, Double, Double)
c (a', b', c', d') = (d, e, 180 - a - b - c, a, b) where
	e = (atand (sc * sind (a + b))/(sc*(cosd (a + b)) + sd))
	sd = (sind d)*sa/sind (180 - a - b - d)
	sc = (sind c)*sb/sind (180 - a - b - c)
	sb = (sind b)/sind (180 - a - b)
	sa = (sind a)/sind (180 - a - b)
	a = fromIntegral a'
	b = fromIntegral b'
	c = fromIntegral c'
	d = fromIntegral d'
	sind a = sin (a*pi/180)
	cosd a = cos (a*pi/180)
	atand r = (atan r) `mod'` pi*180/pi

validAngles (a, b, c, d, e) = 0 < a && 0 < b && 0 < c && 0 < d && 0 < e
	&& b < d + e && a + d + e < 180 && c + d + e < 180

main = $quickCheckAll

