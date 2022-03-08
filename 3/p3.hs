primes = 2 : 3 : seive wheel where
	wheel = [b + o | b <- [6, 12..], o <- [-1, 1]]
	seive (p:ns) = p:filter (\n -> n `mod` p /= 0) ns
factor n = factor_over n primes where
			factor_over n [] = [n]
			factor_over n (p:ps) = if p*p > n
									then if n == 1
										then []
										else [n]
									else if n `mod` p == 0
										then p:factor_over (divide_out n p) ps
										else factor_over n ps
			divide_out n p = if n `mod` p == 0 then divide_out (n `div` p) p else n
main = do
	print $ last $ factor 600851475143

