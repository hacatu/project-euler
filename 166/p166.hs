import Data.List

firstBoard :: [Int]
firstBoard = [0 | _ <- [1..16]]

transforms :: [[Int]]
transforms = [[0,0,0,1,
               0,1,0,0,
               1,0,0,0,
               0,0,1,0],
              [0,0,0,1,
               1,0,0,0,
               0,0,1,0,
               0,1,0,0],
              [0,0,1,0,
               0,1,0,0,
               0,0,0,1,
               1,0,0,0],
              [0,0,1,0,
               1,0,0,0,
               0,1,0,0,
               0,0,0,1],
              [0,1,0,0,
               0,0,0,1,
               0,0,1,0,
               1,0,0,0],
              [1,0,0,0,
               0,0,0,1,
               0,1,0,0,
               0,0,1,0],
              [1,0,0,0,
               0,0,1,0,
               0,0,0,1,
               0,1,0,0],
              [0,1,0,0,
               0,0,1,0,
               1,0,0,0,
               0,0,0,1]] --All 8 rotations of the one unique arrangement with one one in every row, column, and diagonal

addTransforms :: a -> [[Int]] -> [[Int]]
addTransforms _ ns = nub (ns ++ [zipWith (+) n a | a <- transforms, n <- ns])

main :: IO ()
main = putStrLn $ show $ length $ foldr addTransforms [firstBoard] [1..36]
