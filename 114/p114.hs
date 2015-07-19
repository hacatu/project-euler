reb 0 = 0
reb 1 = 0
reb 2 = 0
reb n = 1 + sum [(n - 3 - i)*reb i | i <- [3 .. n - 4]]

b n = 1 + sum [(i + 1)*reb (n - i) | i <- [0 .. n - 3]]

main = putStrLn $ show $ b 50

