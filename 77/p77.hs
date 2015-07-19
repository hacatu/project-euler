a000607 = p a000040_list where
    p _ 0 = 1
    p ks'@(k:ks) m = if m < k then 0 else p ks' (m - k) + p ks m
    a000040_list = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73]

main = putStrLn $ show (a000607 71)
