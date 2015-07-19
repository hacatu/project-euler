fact :: Integer -> Integer
fact 0 = 1
fact n = product [1 .. n]

ways' :: Integer -> Integer -> Integer
ways' squares blocks = (fact (squares + blocks)) `quot` ((fact squares)*(fact blocks))

ways :: Integer -> Integer -> Integer
ways length block = sum [let squares = length - blocks*block in ways' squares blocks | blocks <- [0 .. (length `quot` block)]]

cways :: Integer -> Integer -> Integer -> Integer
cways length a b = (sum [ways length block | block <- [a .. b]]) - (b - a + 1)

main :: IO ()
main = putStrLn $ show $ cways 50 2 4

