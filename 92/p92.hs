ssd :: Int -> Int
ssd 0 = 0
ssd x = o*o + ssd h where
    (h, o) = quotRem x 10

ssdt 89 = 89
ssdt 1 = 1
ssdt x = ssdt $ ssd x

main = do
    print $ length $ [x | x <- [1..10000000], ssdt x == 89]

