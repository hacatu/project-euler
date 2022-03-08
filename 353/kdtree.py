from collections.abc import Mapping, Iterable, Collection
from typing import TypeVar, Callable, Any
from dataclasses import dataclass
from enum import Enum
from functools import partial, cmp_to_key
from math import inf, isinf, atan2, pi
import random as rand
import itertools as itt
import numbers
import operator
import copy
import math
import heapq

WalkDecisionKind = Enum("WalkDecisionKind", "CONTINUE SKIP_CHILDREN STOP")

T = TypeVar('T')
KDPoint = tuple[numbers.Real, ...]
KDEntry = tuple[KDPoint, T]
KDPointComparator = Callable[[KDPoint, KDPoint], int]

def ident(o):
	return o

def swap(l: list[Any], i: int, j: int) -> None:
	l[i], l[j] = l[j], l[i]

def xy_angle(pt):
	return atan2(*pt[:2])%(2*pi)

class MinMaxHeap(Collection):
	def __init__(self, init=None, /, key : Callable[[T], Any] = ident):
		self._elems = list(init) if init is not None else []
		self.key = key
		self.heapify()
	
	def peek_min(self) -> T:
		return self._elems[0]
	
	def peek_max(self) -> T:
		if len(self) < 3:
			return self._elems[-1]
		return max(self._elems[1:3], key=self.key)
	
	def sift_up(self, i: int) -> None:
		if i == 0:
			return
		is_min_layer = bool((i + 1).bit_length()&1)
		i1 = (i - 1)//2
		cmp = operator.lt if is_min_layer else operator.gt
		if cmp(self.key(self._elems[i1]), self.key(self._elems[i])):
			swap(self._elems, i, i1)
			i = i1
			is_min_layer ^= True
			cmp = operator.lt if is_min_layer else operator.gt
		while i > 2:
			i1 = (i - 3)//4
			if cmp(self.key(self._elems[i]), self.key(self._elems[i1])):
				swap(self._elems, i, i1)
				i = i1
			else:
				break

	def sift_down(self, i: int) -> None:
		is_min_layer = bool((i + 1).bit_length()&1)
		exm = min if is_min_layer else max
		cmp = operator.lt if is_min_layer else operator.gt
		while 2*i + 1 < len(self):
			idxs = [2*i + 1, 2*i + 2, 4*i + 3, 4*i + 4, 4*i + 5, 4*i + 6]
			m = exm((idx for idx in idxs if idx < len(self)), key = lambda idx: self.key(self._elems[idx]))
			if m > 2*i + 2:
				if cmp(self.key(self._elems[m]), self.key(self._elems[i])):
					swap(self._elems, m, i)
					p = (m - 1)//2
					if cmp(self.key(self._elems[p]), self.key(self._elems[m])):
						swap(self._elems, m, p)
					i = m
				else:
					break
			else:
				if cmp(self.key(self._elems[m]), self.key(self._elems[i])):
					swap(self._elems, m, i)
				break

	def heapify(self, /, key : Callable[[T], Any] = None) -> None:
		if key is not None:
			self.key = key
		for i in range(len(self._elems) - 1, -1, -1):
			self.sift_down(i)

	def push(self, a: T) -> None:
		self._elems.append(a)
		self.sift_up(len(self._elems) - 1)

	def pop_min(self) -> T:
		return self.pop_idx(0)

	def pop_max(self) -> T:
		if len(self) < 3:
			return self.pop_idx(len(self) - 1)
		return self.pop_idx(1 if self.key(self._elems[1]) > self.key(self._elems[2]) else 2)
	
	def pop_idx(self, i: int) -> T:
		if i == len(self) - 1:
			return self._elems.pop()
		res = self._elems[i]
		self._elems[i] = self._elems.pop()
		self.sift_down(i)
		return res

	def pushpop_min(self, a: T) -> T:
		self.push(a)
		return self.pop_min()

	def pushpop_max(self, a: T) -> T:
		self.push(a)
		return self.pop_max()

	def __contains__(self, a):
		# TODO: this implementation only checks if a is in bounds once
		if len(self._elems) < 3:
			return any(e == a for e in self._elems)
		k = self.key(a)
		if k < self.key(self._elems[0]):
			return False
		if k > max(map(self.key, self._elems[1:3])):
			return False
		return any(e == a for e in self._elems)
	
	def __iter__(self):
		return map(operator.itemgetter(1), self._elems)
	
	def __len__(self):
		return len(self._elems)

@dataclass
class KDWindow:
	bl: KDPoint
	tr: KDPoint

	def split(self, root: KDPoint, depth: int) -> 'tuple[KDWindow, KDWindow]':
		idx = depth%len(root)
		res = (copy.deepcopy(self), copy.deepcopy(self))
		res[0].tr[idx] = root[idx]
		res[1].bl[idx] = root[idx]
		return res
	
	def update(self, a: KDPoint) -> None:
		dim = len(self.bl)
		for i in range(dim):
			if self.cmp_depth(i, a, self.bl) < 0:
				self.bl[i] = a[i]
			elif self.cmp_depth(i, a, self.tr) > 0:
				self.tr[i] = a[i]
	
	def min_sqdist(self, a: KDPoint) -> numbers.Real:
		return sum(max(l - i, i - r, 0)**2 for (l, r, i) in zip(self.bl, self.tr, a))

	@staticmethod
	def cmp_depth(depth: int, a: KDPoint, b: KDPoint) -> int:
		idx = depth%len(a)
		key_a = a[idx]
		key_b = b[idx]
		if key_a < key_b:
			return -1
		if key_a > key_b:
			return 1
		return 0

class KDSpWindow(KDWindow):
	def split(self, root, depth):
		dim = len(root) - 1
		idx = depth%dim
		res = (copy.deepcopy(self), copy.deepcopy(self))
		if idx == dim - 1:
			if root[0] != 0 or root[1] != 0:
				res[0].tr[:2] = root[:2]
				res[1].bl[:2] = root[:2]
		else:
			res[0].tr[2 + idx] = root[2 + idx]
			res[1].bl[2 + idx] = root[2 + idx]
		return res
	
	def update(self, a):
		dim = len(self.bl) - 1
		for i in range(dim - 1):
			if self.cmp_depth(i, a, self.bl) < 0:
				self.bl[2 + i] = a[2 + i]
			elif self.cmp_depth(i, a, self.tr) > 0:
				self.tr[2 + i] = a[2 + i]
		if self.cmp_depth(dim - 1, a, self.bl) < 0:
			self.bl[:2] = a[:2]
		elif self.cmp_depth(dim - 1, a, self.tr) > 0:
			self.tr[:2] = a[:2]
	
	def min_sqdist(self, a):
		# The min sqdist on a sphere is the same in the last n-2 dimensions,
		# but the first 2 dimensions which are treated as an angle must have their
		# sqdist computed differently
		# The distance beween x1, x2 and l1, l2 can be found by the law of cosines
		# sqdist = 2*x1**2 + 2*x2**2 - 2*(x1**2 + x2**2)*cos
		# Here, x1**2 + x2**2 is the square of the radius of the circle in the hypersphere
		# where x1, x2 lives.  The triangle in question in the law of cosines
		# is in the circle with x1, x2 and is an isosolese triangle through the center
		# and two points on the circumference
		# Thus the two known sides are the radius of that circle and the cos can be
		# found through the dot product
		# sqdist = 2*(x1**2 + x2**2 - (x1*l1 + x2*l2)/(l1**2 + l2**2))
		# Remember, this is just the component of the sqdist in the first two dimensions
		# Also notice that this is not necessarily an integer, so we will use floor division
		# and accept that we explore some windows that are actually too far to contribute to the
		# k closest points
		#
		# Now we just have to find whether x1, x2 is closer to l1, l2, r1, r2, or is between them
		dim = len(self.bl) - 1
		sqdist = -1
		if self.cmp_depth(dim - 1, self.bl, self.tr) > 0:
			if self.cmp_depth(dim - 1, self.bl, a) <= 0:
				sqdist = 0
			if self.cmp_depth(dim - 1, a, self.tr) <= 0:
				sqdist = 0
		elif self.bl[1] == self.tr[1] == 0:
			sqdist = 0
		else:
			if self.cmp_depth(dim - 1, self.bl, a) <= 0 and self.cmp_depth(dim - 1, a, self.tr) <= 0:
				sqdist = 0
		if sqdist == -1:
			lcrs = self.bl[0]**2 + self.bl[1]**2
			rcrs = self.tr[0]**2 + self.tr[1]**2
			acrs = a[0]**2 + a[1]**2
			lda = a[0]*self.bl[0] + a[1]*self.bl[1]
			rda = a[0]*self.tr[0] + a[1]*self.tr[1]
			if lcrs*rda < rcrs*lda:
				sqdist = 2*(acrs - rda//rcrs)
			else:
				sqdist = 2*(acrs - lda//lcrs)
		return sqdist + sum(max(l - i, i - r, 0)**2 for (l, r, i) in itt.islice(zip(self.bl, self.tr, a), 2, None))

	@staticmethod
	def cmp_depth(depth, a, b):
		dim = len(a) - 1
		idx = depth%dim
		if idx == dim - 1:
			a_x, a_y = a[:2]
			b_x, b_y = b[:2]
			# if a and b are in opposite half planes
			if (a_y >= 0) != (b_y >= 0):
				if a_y < 0:
					# if (0, 0) should compare equal to all directions, the following line is needed
					# return 0 if (b_x, b_y) == (0, 0) else 1
					# if (0, 0) should compare less than all directions, the following line is needed
					return 1
				# if (0, 0) == all
				# return 0 if (a_x, a_y) == (0, 0) else -1
				# if (0, 0) < all
				return -1
			crossp = a_x*b_y - b_x*a_y
			if crossp > 0:
				return -1
			if crossp < 0:
				return 1
			# if (0, 0) == all
			#return 0
			# if (0, 0) < all
			if (a_x, a_y) == (0, 0):
				return -1 if (b_x, b_y) != (0, 0) else 0
			return 0 if (b_x, b_y) != (0, 0) else 1
		key_a = a[2 + idx]
		key_b = b[2 + idx]
		if key_a < key_b:
			return -1
		if key_a > key_b:
			return 1
		return 0

KDVisitor = Callable[[KDEntry, KDWindow, object], WalkDecisionKind]

VEC_ISORT_BOUND = 16

class KDkClosestState:
	def __init__(self, pt: KDPoint, k: int):
		self.pt = pt
		self.k = k
		self.points = MinMaxHeap(key = lambda ent: self.sqdist(ent[0]))
		self.max_sqdist = inf
	
	def sqdist(self, pt: KDPoint) -> numbers.Real:
		return sum((b - a)**2 for (a, b) in zip(pt, self.pt))

	def visit(self, ent: KDEntry):
		if len(self.points) < self.k:
			self.points.push(ent)
		else:
			self.points.pushpop_max(ent)
			self.max_sqdist = self.sqdist(self.points.peek_max()[0])

class KDTree(Mapping):
	def __init__(self, init=None, *, bounds=None, spherical=False):
		if init is None:
			init = {}
		if isinstance(init, Mapping):
			self.points = list(init.items())
		elif isinstance(init, Iterable):
			self.points = list(init)
			if not (all(isinstance(decl, tuple) and len(decl) == 2 for decl in self.points)):
				raise TypeError("'init' must be a Mapping (or Iterable of pairs) where the keys (or first values) are points in space, ie tuples of real numbers")
		else:
			raise TypeError("'init' must be a Mapping (or Iterable of pairs) where the keys (or first values) are points in space, ie tuples of real numbers")
		if bounds is None:
			if len(self.points) == 0:
				raise TypeError("Cannot infer dimension (no points or bounds provided)")
			pt = self.points[0][0]
			if spherical:
				bounds = KDSpWindow(list(pt), list(pt))
			else:
				bounds = KDWindow(list(pt), list(pt))
			for pt, _ in self.points[1:]:
				bounds.update(pt)
		self.bounds = bounds
		self._treeify(0, 0, len(self))
		if not self._check_tree(0, 0, len(self)):
			raise AssertionError("KD Tree invariant error")

	def _strided_sort_points(self, a: int, b: int, stride: int, cmp: KDPointComparator) -> None:
		for ii in range(a + stride, b, stride):
			for jj in range(ii, a, -1):
				if cmp(self.points[jj - stride][0], self.points[jj][0]) <= 0:
					break
				swap(self.points, jj, jj - stride)

	def _points_pivot_mm(self, a: int, b: int, cmp: KDPointComparator) -> int:
		if a >= b:
			return -1
		chunk_size = 5
		chunk_stride = 1
		while chunk_stride < b - 1:
			for ii in range(a, b, chunk_size):
				curr_size = chunk_size
				if ii + curr_size > b:
					curr_size = b - ii
				curr_len = (curr_size + chunk_stride - 1)//chunk_stride
				if curr_len == 1:
					break
				self._strided_sort_points(ii, curr_size, chunk_stride, cmp)
				if curr_len == 2:
					break
				jj = ii + (curr_len - 1)//2*chunk_stride
				self.points[ii], self.points[jj] = self.points[jj], self.points[ii]
			chunk_stride = chunk_size
			chunk_size *= 5
		return a

	def _points_pivot_m3(self, a: int, b: int, cmp: KDPointComparator) -> int:
		if a >= b:
			return -1
		mid_idx = (a + b)//2
		cmpr = cmp(self.points[a][0], self.points[mid_idx][0])
		if cmpr == 0:
			return mid_idx
		if sorted := cmpr*cmp(self.points[mid_idx][0], self.points[b - 1][0]):
			if sorted > 0:
				return mid_idx
			if cmp(self.points[b - 1][0], self.points[a][0]) >= 0:
				return b - 1
			return a
		return mid_idx

	def _partition_points(self, a: int, b: int, piv_idx: int, cmp: KDPointComparator) -> int:
		if a >= b or piv_idx < a or piv_idx >= b:
			return -1
		if piv_idx != b - 1:
			swap(self.points, piv_idx, b - 1)
		ii = a - 1
		jj = b - 1
		while True:
			while True:
				ii += 1
				if ii >= jj or cmp(self.points[ii][0], self.points[b - 1][0]) >= 0:
					break
			if ii == jj:
				break
			while True:
				jj -= 1
				if ii >= jj or cmp(self.points[jj][0], self.points[b - 1][0]) < 0:
					break
			if ii == jj:
				break
			swap(self.points, ii, jj)
		if jj != b - 1:
			swap(self.points, jj, b - 1)
		if not (all(cmp(pt, self.points[jj][0]) < 0 for (pt, _) in self.points[a:jj])
			and all(cmp(pt, self.points[jj][0]) >= 0 for (pt, _) in self.points[jj + 1:b])):
			raise AssertionError("Partition Invalid")
		return jj
	
	def _partition_with_median(self, a: int, b: int, med_idx: int, cmp: KDPointComparator) -> int:
		if a >= b or med_idx < a or med_idx >= b:
			raise IndexError()
			return -1
		# We need to partition our array but ensure the median element is placed in the center
		# of the array.  This requires a partitioning algorithm that can handle equal elements to
		# the median, since the basic partitioning algorithm could have it out of place by up to
		# as many equal elements as there are
		lb = a # the elements < the median occur in the range [a, lb)
		ea = (b + a)//2
		eb = ea + 1 # the elements == the median occur in the range [ea, eb)
		ha = b # the elements > the median occur in the range [ha, b)
		med_pt = self.points[med_idx][0]
		#print("partitioning with median [{}]={}".format(med_idx, med_pt))
		if med_idx != ea:
			swap(self.points, ea, med_idx) # ensure the median is placed at the center
			med_idx = ea
		# now we will extend [a, lb), [ha, b), and [ea, eb) until they encompass the entire array
		# TODO: consider alternating which side we place == elements on
		#print("pwm: l[a,{}) e[{},{}) h[{},b)".format(lb, ea, eb, ha))
		while lb < ea and eb < ha:
			# first we skip over any elements on the left < the median, and swap any == to the
			# central region
			while lb < ea and (cmpr := cmp(self.points[lb][0], med_pt)) <= 0:
				if cmpr < 0:
					lb += 1
				else:
					ea -= 1
					if lb == ea:
						break
					swap(self.points, lb, ea)
			# next we skip over any elements on the right > the median, and swap any == to the
			# central region
			while eb < ha and (cmpr := cmp(med_pt, self.points[ha - 1][0])) <= 0:
				if cmpr < 0:
					ha -= 1
				else:
					eb += 1
					if eb == ha:
						break
					swap(self.points, eb - 1, ha - 1)
			if lb == ea or eb == ha:
				break
			ha -= 1
			swap(self.points, lb, ha)
			lb += 1
		# now either eb == ha or lb == ea so only one of the following two loops will run
		#print("pwm: l[a,{}) e[{},{}) h[{},b)".format(lb, ea, eb, ha))
		while lb < ea:
			if (cmpr := cmp(self.points[lb][0], med_pt)) < 0:
				lb += 1
			elif cmpr == 0:
				ea -= 1
				if lb == ea:
					break
				swap(self.points, lb, ea)
			else: # lb is > the median so it must go on the right, so we must shift an == element over
				ea -= 1
				eb -= 1
				ha -= 1
				if lb == ea:
					swap(self.points, ea, ha)
					break
				self.points[lb], self.points[ea], self.points[ha] = self.points[ea], self.points[ha], self.points[lb]
		#print("pwm: l[a,{}) e[{},{}) h[{},b)".format(lb, ea, eb, ha))
		while eb < ha:
			if (cmpr := cmp(med_pt, self.points[ha - 1][0])) < 0:
				ha -= 1
			elif cmpr == 0:
				eb += 1
				if eb == ha:
					break
				swap(self.points, eb - 1, ha - 1)
			else: # ha is < the median so it must go on the left, so we must shift an == element over
				lb += 1
				ea += 1
				eb += 1
				if eb == ha:
					swap(self.points, ea - 1, ha - 1)
					break
				self.points[lb - 1], self.points[eb - 1], self.points[ha - 1] = self.points[ha - 1], self.points[lb - 1], self.points[eb - 1]
		#print("pwm: l[a,{}) e[{},{}) h[{},b)".format(lb, ea, eb, ha))
		if lb != ea or eb != ha:
			raise AssertionError("Partition Incomplete")
		if not (all(cmp(pt, med_pt) < 0 for (pt, _) in self.points[a:lb])
			and all(cmp(pt, med_pt) == 0 for (pt, _) in self.points[ea:eb])
			and all(cmp(pt, med_pt) > 0 for (pt, _) in self.points[ha:b])):
			raise AssertionError("Partition Invalid")
		return med_idx

	def _sort_points_end(self, a: int, b: int, i: int, cmp: KDPointComparator) -> int:
		which = 1
		if i < 0:
			which = -1
			i = -i - 1
		i += a
		if i >= b:
			return -1
		for ii in range(a + 1, i + 1):
			for jj in range(ii, a, -1):
				if which*cmp(self.points[jj - 1][0], self.points[jj][0]) <= 0:
					break
				swap(self.points, jj, jj - 1)
		for ii in range(i + 1, b):
			if which*cmp(self.points[i][0], self.points[ii][0]) <= 0:
				continue
			swap(self.points, i, ii)
			for jj in range(i, a, -1):
				if which*cmp(self.points[jj - 1][0], self.points[jj][0]) <= 0:
					break
				swap(self.points, jj, jj - 1)
		return i

	def _points_ith(self, a: int, b: int, i: int, cmp: KDPointComparator) -> int:
		"""Get the ith point in self.points[a:b], where i is counted from a.
		To be clear, a is the inclusive start index of the subrange and b is the exclusive
		stop index of the subrange.  An i of 0 means to find the smallest element in the
		subrange, 1 means second smallest, and so on, up to b - a - 1 indicating the
		largest"""
		while True:
			if i >= b - a:
				raise IndexError("i out of range")
				return -1
			if i < VEC_ISORT_BOUND:
				#print("points_ith: finishing by sorting array start ({})".format(i))
				return self._sort_points_end(a, b, i, cmp)
			if b - a - i < VEC_ISORT_BOUND + 1:
				#print("points_ith: finishing by sorting array end ({})".format(i - b))
				return self._sort_points_end(a, b, i - b + a, cmp)
			piv_idx = self._points_pivot_m3(a, b, cmp)
			piv_idx = self._partition_points(a, b, piv_idx, cmp)
			if piv_idx == -1:
				return -1
			if i < piv_idx - a:
				b = piv_idx
			elif i > piv_idx - a:
				i -= piv_idx - a + 1
				a = piv_idx + 1
			else:
				return piv_idx

	def _check_tree(self, depth: int, a: int, b: int) -> bool:
		while b > a:
			mid_idx = (a + b)//2
			mid = self.points[mid_idx][0]
			cmp = partial(self.bounds.cmp_depth, depth)
			if not all(cmp(pt, mid) <= 0 for (pt, _) in self.points[a:mid_idx]):
				return False
			if not all(cmp(pt, mid) >= 0 for (pt, _) in self.points[mid_idx + 1:b]):
				return False
			depth += 1
			if not self._check_tree(depth, mid_idx + 1, b):
				return False
			b = mid_idx
		return True

	def _treeify(self, depth: int, a: int, b: int) -> None:
		while b > a:
			mid_idx = (a + b)//2
			cmp = partial(self.bounds.cmp_depth, depth)
			#print("[{}, {})@{}".format(a, b, mid_idx))
			piv_idx = self._points_ith(a, b, mid_idx - a, cmp)
			mid_pt = [pt for (pt, _) in self.points[a:b]]
			mid_pt.sort(key=cmp_to_key(cmp))
			mid_pt = mid_pt[mid_idx - a]
			if cmp(mid_pt, self.points[piv_idx][0]):
				raise AssertionError("_points_ith failed")
			#print("treeify [{}]={}".format(piv_idx, self.points[piv_idx][0]))
			piv_idx = self._partition_with_median(a, b, piv_idx, cmp)
			#print("med at {}".format(piv_idx))
			if piv_idx != mid_idx:
				raise AssertionError("partition did not place median at center")
			depth += 1
			self._treeify(depth, mid_idx + 1, b)
			b = mid_idx

	def walk(self, visitor: KDVisitor, out: object) -> None:
		window = self.bounds
		self._walk_r(visitor, out, window, 0, 0, len(self.points))
	
	def _walk_r(self, visitor: KDVisitor, out: object, window: KDWindow, depth: int, a: int, b: int) -> WalkDecisionKind:
		while b > a:
			mid_idx = (a + b)//2
			ent =  self.points[mid_idx]
			root = ent[0]
			decision = visitor(ent, window, out)
			if decision == WalkDecisionKind.STOP:
				return decision
			elif decision == WalkDecisionKind.SKIP_CHILDREN:
				return WalkDecisionKind.CONTINUE
			subwindows = window.split(root, depth)
			decision = self._walk_r(visitor, out, subwindows[0], depth + 1, a, mid_idx)
			if decision == WalkDecisionKind.STOP:
				return decision
			a = mid_idx + 1
			depth += 1
			window = subwindows[1]
		return WalkDecisionKind.CONTINUE

	def _getidx(self, pt: KDPoint, depth: int, a: int, b: int) -> int:
		while b > a:
			mid_idx = (a + b)//2
			root, _ = self.points[mid_idx]
			cmpr = self.bounds.cmp_depth(depth, pt, root)
			depth += 1
			if cmpr < 0:
				b = mid_idx
				continue
			if cmpr > 0:
				a = mid_idx + 1
				continue
			if root == pt:
				return mid_idx
			res = self._getidx(pt, depth, a, mid_idx)
			if res != -1:
				return res
			a = mid_idx + 1
		return -1

	def __getitem__(self, pt: KDPoint) -> T:
		idx = self._getidx(pt, 0, 0, len(self.points))
		if idx == -1:
			raise KeyError(pt)
		return self.points[idx][1]
	
	def __iter__(self):
		return self.keys()
	
	def __len__(self):
		return len(self.points)
	
	def __contains__(self, pt: KDPoint):
		return self._getidx(pt, 0, 0, len(self.points)) != -1
	
	def keys(self):
		return iter(map(operator.itemgetter(0), self.points))
	
	def items(self):
		return iter(map(operator.itemgetter(1), self.points))
	
	def values(self):
		return iter(self.points)
	
	def get(self, pt: KDPoint, default : T = None) -> T:
		try:
			return self[pt]
		except KeyError:
			return default
	
	# TODO: This is an O(nlogn) algorithm, it may be possible to get closer to O(n)
	def __eq__(self, other):
		if not isinstance(other, KDTree):
			return False
		if (len(self.points) != len(other.points) or
			type(self.bounds) is not type(other.bounds) or
			len(self.bounds.tr) != len(other.bounds.tr)):
			return False
		sentinel = object()
		for pt, val in self.items():
			val1 = other.get(pt, sentinel)
			if val1 is sentinel:
				return False
			if val1 != val:
				return False
		return True
	
	def __ne__(self, other):
		return not self.__eq__(other)

	@staticmethod
	def _k_closest_visitor(ent: KDEntry, window: KDWindow, out: KDkClosestState) -> WalkDecisionKind:
		#("Visiting: {}".format(ent))
		#print("window.bl: {}".format(window.bl))
		#print("window.tr: {}".format(window.tr))
		out.visit(ent)
		#print("out.max_sqdist = {}".format(out.max_sqdist))
		#print("window.min_sqdist = {}".format(window.min_sqdist(ent[0])))
		if isinf(out.max_sqdist) or out.max_sqdist > window.min_sqdist(ent[0]):
			return WalkDecisionKind.CONTINUE
		return WalkDecisionKind.SKIP_CHILDREN

	def k_closest(self, pt: KDPoint, k: int) -> list[KDEntry]:
		state = KDkClosestState(pt, k)
		#print(state.points)
		self.walk(self._k_closest_visitor, state)
		return state.points._elems
	
	def _k_closest_naive(self, pt: KDPoint, k: int) -> list[KDEntry]:
		return heapq.nsmallest(k, self.points, key=lambda ent: sum((x1-x2)**2 for (x1, x2) in zip(pt, ent[0])))

def rand_sphere_points(r: numbers.Real, n: int) -> list[numbers.Real]:
	res = [None]*n
	for i in range(n):
		h = rand.uniform(-r, r)
		a = rand.uniform(0, 2*math.pi)
		d = math.sqrt(r**2 - h**2)
		res[i] = (d*math.cos(a), h, d*math.sin(a))
	return res

perfect_sqrts = {x*x: x for x in range(2**15)}

def pythag_triples(r):
	yield (r, 0, 0)
	sqr = r*r
	l = int(math.floor(math.sqrt(sqr/3)))
	for a in range(r - 1, l, -1):
		for b in range(min(a, int(math.floor(math.sqrt(sqr - a*a)))), int(math.ceil(math.sqrt((sqr - a*a)/2))) - 1, -1):
			c = perfect_sqrts.get(sqr - a*a - b*b)
			if c is not None:
				yield (a, b, c)

def pythag_sphere_slice_points(r):
	res = []
	for pt in pythag_triples(r):
		new_points = set(itt.permutations(pt))
		new_points |= {(x, y, -z) for (x, y, z) in new_points}
		res += list(new_points)
	return res
