import Data.Array

sigma_x_sieve x buf p = if p > (snd $ bounds buf)
                        then buf
                        else if buf!p /= 1
                             then sigma_x_sieve x buf (p + 1)
                             else sigma_x_sieve x (buf // [(m, buf!m * sigma_x_factor x m p) | m <- [p,2*p..snd $ bounds buf]]) (p + 1)

sigma_x_factor x m p = div (p^(x*a + x) - 1) (p^x - 1)
                       where a = max_pow m p

max_pow m p = if m `mod` p == 0
              then 1 + max_pow (m `div` p) p
              else 0

is_square n = n == (truncate $ sqrt $ fromIntegral n)^2

sum_square_sigma_2s m = sum $ filter (is_square.(sigma_xs!)) [1..m]
                        where sigma_xs = sigma_x_sieve 2 (array (1, m) [(i, 1) | i <- [1..m]]) 2

main = do
    print $ sum_square_sigma_2s 640000

