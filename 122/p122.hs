minSums :: Integer -> Integer
minSums 1 = 0
minSums n = 1 + minimum [(minSums (n - i)) + minSums i | i <- [1, n - 1]]

main :: IO ()
main = putStrLn $ show $ sum [minSums n | n <- [1 .. 200]]

