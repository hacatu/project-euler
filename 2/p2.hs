main = putStrLn $ show $ sum $ takeWhile (<4000000) $ map fst $ iterate (\(a, b)->(3*a + 2*b, 2*a + b)) (2, 1)
