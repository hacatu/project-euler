import Control.Monad
import Control.Monad.ST
import Data.Array.ST

{-sigma_x_sieve x buf p = if p > (snd $ bounds buf)
                        then buf
                        else if buf!p /= 1
                             then sigma_x_sieve x buf (p + 1)
                             else sigma_x_sieve x (buf // [(m, buf!m * sigma_x_factor x m p) | m <- [p,2*p..snd $ bounds buf]]) (p + 1)

sigma_x_factor x m p = div (p^(x*a + x) - 1) (p^x - 1)
                       where a = max_pow m p

sum_square_sigma_2s m = sum $ filter (is_square.(sigma_xs!)) [1..m]
                        where sigma_xs = sigma_x_sieve 2 (array (1, m) [(i, 1) | i <- [1..m]]) 2

main = do
    print $ sum_square_sigma_2s 640000-}

is_square n = n == (truncate $ sqrt $ fromIntegral n)^2

max_pow m p = if m `mod` p == 0
              then 1 + max_pow (m `div` p) p
              else 0

make_buffer :: Integer -> ST s (STArray s Integer Integer)
make_buffer m = newArray (1, m) 1

mark_x_factor :: Integer -> ST s (STArray s Integer Integer) -> Integer -> Integer -> ST s (STArray s Integer Integer)
mark_x_factor x sbuf p m = do
                           buf <- sbuf
                           e <- readArray buf m
                           writeArray buf m 1
                           return buf
                           where
                           d = div (p^(x*a + x) - 1) (p^x - 1)
                           a = max_pow m p

sigma_x_sieve :: Integer -> ST s (STArray s Integer Integer) -> Integer -> ST s (STArray s Integer Integer)
sigma_x_sieve x sbuf p = do
                        buf <- sbuf
                        m <- fmap snd (getBounds buf)
                        bufp <- readArray buf p
                        if p > m
                        then return buf
                        else if bufp /= 1
                             then sigma_x_sieve x (return buf) (p + 1)
                             else (sigma_x_factor x (return buf) p p) >> (sigma_x_sieve x (return buf) (p + 1))

sigma_x_factor :: Integer -> ST s (STArray s Integer Integer) -> Integer -> Integer -> ST s (STArray s Integer Integer)
sigma_x_factor x sbuf p m = do
                           buf <- sbuf
                           m <- fmap snd (getBounds buf)
                           if p > m
                           then return buf
                           else (mark_x_factor x (return buf) p m) >> (sigma_x_factor x (return buf) p (m + p))

helper :: [(Integer, Integer)] -> Integer
helper ls = sum (map fst (filter (is_square.snd) ls))

sum_square_sigma_2s :: Integer -> ST s Integer
sum_square_sigma_2s m = do
                        buf <- sigma_x_sieve 2 (make_buffer m) 2
                        ps <- getAssocs buf
                        return $ helper ps

main = print $ runST $ sum_square_sigma_2s 64000000

