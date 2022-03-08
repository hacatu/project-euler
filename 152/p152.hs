firstGreedy :: Integer -> [(Integer, Integer)] -> Maybe [Integer]
firstGreedy 0 _ = Just []
firstGreedy t xrs = firstGreedy' t $ dropWhile ((>t) . fst) xrs

firstGreedy' :: Integer -> [(Integer, Integer)] -> Maybe [Integer]
firstGreedy' 0 _ = Just [] --Dummy case
firstGreedy' t [] = Nothing --Dummy case
firstGreedy' t [(x, _)] = if t == x then Just [x] else Nothing
firstGreedy' t ((x, r):xrs) = if t > r then
                                Nothing
                            else
                                case firstGreedy (t - x) xrs of
                                    Just a -> Just (x:a)
                                    Nothing -> firstGreedy' t xrs

tailsums :: [Integer] -> [Integer]
tailsums nums = foldl addcat [0] nums where
                addcat :: [Integer] -> Integer -> [Integer]
                addcat (l@(x:_)) n = (x+n):l

main :: IO ()
main = putStrLn $ show $ fmap (map (\x->floor . sqrt . fromInteger $ div denom x)) $ firstGreedy half $ zip nums tails where
        denom = foldr lcm 1 [x*x|x<-[2..55]]
        half = denom `div` 2
        nums = [denom `div` (x*x)|x<-[2..55]]
        tails = tailsums nums

