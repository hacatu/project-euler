from kdtree import *

"""
_strided_sort_points
_points_pivot_mm
_points_pivot_m3
_partition_points
_partition_with_median
_sort_points_end
_points_ith
_treeify
"""

"""
for n in range(1, 16):
	R = 2**n - 1
	print(f"R={R}")

	init = [(pt, None) for pt in pythag_sphere_slice_points(R)]
	spwindow = KDSpWindow([R, 0, -R], [0, R, R])
	latt_tree = KDTree(init, bounds=spwindow)

	min_a = (0, 0, R)
	min_b = (0, R, 0)
	min_sqdist = 2*R**2
	for a in latt_tree.keys():
		_, (b, _) = latt_tree._k_closest_naive(a, 2)
		sqdist = sum((x - y)**2 for (x, y) in zip(a, b))
		if sqdist < min_sqdist:
			min_a = a
			min_b = b
			min_sqdist = sqdist
	print(f"min_sqdist={min_sqdist}: {min_a}-{min_b}")
"""

for n in range(5, 16):
	R = 2**n - 1
	print(f"R={R}")

	init = [(pt, None) for pt in pythag_sphere_slice_points(R)]
	spwindow = KDSpWindow([R, 0, -R], [0, R, R])
	latt_tree = KDTree(init, bounds=spwindow)

	for pt in latt_tree.keys():
		epts = latt_tree._k_closest_naive(pt, 2)
		if len(epts) != 2:
			raise AssertionError("_k_closest_naive(pt, k) did not return k entries")
		(ea, _), (eb, _) = epts
		opts = latt_tree.k_closest(pt, 2)
		if len(opts) != 2:
			raise AssertionError("k_closest(pt, k) did not return k entries")
		(oa, _), (ob, _) = opts
		eb_sqdist = sum((x - y)**2 for (x, y) in zip(pt, eb))
		if ea == oa:
			ob_sqdist = sum((x - y)**2 for (x, y) in zip(pt, ob))
		elif ea == ob:
			ob_sqdist = sum((x - y)**2 for (x, y) in zip(pt, oa))
		if ob_sqdist != eb_sqdist:
			raise AssertionError("k_closest did not match _k_closest_naive")

