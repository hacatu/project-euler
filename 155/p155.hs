import Data.Set.Monad (Set, union, empty)
import qualified Data.Set.Monad as Set
import Data.Map (Map, (!))
import qualified Data.Map as Map
import Data.Ratio (Ratio, (%))

allValues :: Int -> Set Rational
allValues n = (allValuesMap n) ! n

allValuesMap :: Int -> Map Int (Set Rational)
allValuesMap n = foldl combine1 (Map.singleton 1 $ Set.singleton (1%1)) [1 .. n]

combine1 :: Map Int (Set Rational) -> Int -> Map Int (Set Rational)
combine1 vss i = Map.insert i (newValues vss i) vss

newValues :: Map Int (Set Rational) -> Int -> Set Rational
newValues vss n = foldl (combine2 vss) empty $ partitions n

combine2 :: Map Int (Set Rational) -> Set Rational -> [Int] -> Set Rational
combine2 vss vs p = union vs $ newValuesP vss p

newValuesP :: Map Int (Set Rational) -> [Int] -> Set Rational
newValuesP vss p = unpack $ foldl combine (Set.singleton (0, 0)) [vss ! n | n <- p]

unpack :: Set (Rational, Rational) -> Set Rational
unpack t = foldl (\a (s, p) -> Set.insert s $ Set.insert (1/p) a) empty $ t

combine :: Set (Rational, Rational) -> Set Rational -> Set (Rational, Rational)
combine sps vs = add <$> vs <*> sps

add :: Rational -> (Rational, Rational) -> (Rational, Rational)
add a (s, p) = (s + a, p + 1/a)

partitions :: Int -> [[Int]]
partitions n = concat [[p:os | os <- partitions (n - p)] | p <- [1 .. n]]

main :: IO ()
main = putStrLn $ show $ length $ allValues 3

