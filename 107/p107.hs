import Data.List

graph :: [[Int]]
graph = [
			[0,16,12,21,0,0,0],
			[16,0,0,17,20,0,0],
			[12,0,0,28,0,31,0],
			[21,17,28,0,18,19,23],
			[0,20,0,18,0,0,11],
			[0,0,31,19,0,0,27],
			[0,0,0,23,11,27,0]
		]

makeEdges :: [[Int]]->Int->[(Int, Int, Int)]
makeEdges graph node = [(node, other, length) | (other, length) <- zip [0..] (graph!!node), length /= 0]

updateEdges :: [[Int]]->[(Int, Int, Int)]->Int->[Int]->[(Int, Int, Int)]
updateEdges graph edges node nodes = [edge | edge@(_, other, _) <- edges++(makeEdges graph node), not (other `elem` nodes)]

pickMin :: ([[Int]], [Int], [(Int, Int, Int)], Int)->([[Int]], [Int], [(Int, Int, Int)], Int)
pickMin (graph, nodes, edges, _) = (graph, nodes', updateEdges graph edges node nodes', length)
									where
									compareEdges :: (Int, Int, Int)->(Int, Int, Int)->Ordering
									compareEdges (_, _, a) (_, _, b) = compare a b
									node :: Int
									length :: Int
									(_, node, length) = minimumBy compareEdges edges
									nodes' :: [Int]
									nodes' = node:nodes

unfold :: (t->Bool)->(t->t)->t->[t]
unfold isDone makeNext start = if isDone next then [start]++[next] else [start]++(unfold isDone makeNext next)
								where
								next = makeNext start

minSpanTreeWt :: [[Int]]->Int
minSpanTreeWt graph = sum [length | (_, _, _, length) <- unfold isDone pickMin (graph, [0], makeEdges graph 0, 0)]
						where
						isDone :: ([[Int]], [Int], [(Int, Int, Int)], Int)->Bool
						isDone (_, nodes, _, _) = (length nodes) == (length graph)

main = putStrLn $ show ((sum [sum row | row <- graph]) `div` 2 - minSpanTreeWt graph)

