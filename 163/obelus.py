from collections import defaultdict
import itertools
from math import floor, ceil
from fractions import Fraction as Q
from numbers import Number, Rational, Integral
Q0 = Q(0)
Q1 = Q(1)

class Var:
	def __init__(self, name):
		self.name = name
	
	def __add__(self, other):
		return Expr("add", self).__add__(other)
	
	def __sub__(self, other):
		return Expr("add", self).__sub__(other)
	
	def __mul__(self, other):
		return Expr("mul", self).__mul__(other)
	
	def __truediv__(self, other):
		return Expr("mul", self).__truediv__(other)
	
	def __mod__(self, other):
		return Expr("mul", self).__mod__(other)
	
	def __pow__(self, other):
		return Expr("mul", self).__pow__(other)
	
	def __radd__(self, other):
		return Expr("add", self).__radd__(other)
	
	def __rsub__(self, other):
		return Expr("add", self).__rsub__(other)
	
	def __rmul__(self, other):
		return Expr("mul", self).__rmul__(other)
	
	def __rtruediv__(self, other):
		return Expr("mul", self).__rtruediv__(other)
	
	def __rmod__(self, other):
		return Expr("mul", self).__rmod__(other)
	
	def __rpow__(self, other):
		return Expr("mul", self).__rpow__(other)
	
	def __neg__(self):
		return Expr("mul", self, -1)
	
	def __pos__(self):
		return self
	
	def __floor__(self):
		return Expr("floor", self)
	
	def __ceil__(self):
		return Expr("ceil", self)
	
	def __str__(self):
		return self.name
	
	def __repr__(self):
		return "Var(" + self.name + ")"
	
	def __eq__(self, other):
		if not isinstance(other, Var):
			return False
		return self.name == other.name
	
	def __hash__(self):
		return hash(self.name)

class Expr:
	def __init__(self, op, *args):
		self.op = op
		self.args = args

	def __add__(self, other):
		if not isinstance(other, Expr):
			other = Expr("add", other)
		if self.op == "add":
			args = self.args
		else:
			args = [self]
		if other.op == "add":
			args += other.args
		else:
			args.append(other)
		return Expr("add", *args)
	
	def __sub__(self, other):
		if not isinstance(other, Expr):
			other = Expr("add", other)
		if self.op == "add":
			args = self.args
		else:
			args = (self,)
		if other.op == "add":
			args += tuple(-a for a in other.args)
		else:
			args += (-other,)
		return Expr("add", *args)
	
	def __mul__(self, other):
		if not isinstance(other, Expr):
			other = Expr("mul", other)
		if self.op == "mul":
			args = self.args
		else:
			args = (self,)
		if other.op == "mul":
			args += other.args
		else:
			args += (other,)
		return Expr("mul", *args)
	
	def __truediv__(self, other):
		if not isinstance(other, Expr):
			other = Expr("mul", other)
		if self.op == "mul":
			if len(self.args) == 1:
				numerator = self.args[0]
			else:
				numerator = self
			denominator = None
		elif self.op == "div":
			numerator, denominator = self.args
		else:
			numerator = self
			denominator = None
		if other.op == "mul":
			if len(other.args) == 1:
				if denominator is None:
					denominator = other.args[0]
				else:
					denominator = Expr("mul", denominator, other.args[0])
			elif denominator is None:
				denominator = other
			else:
				denominator = Expr("mul", denominator, other)
		elif other.op == "div":
			numerator = numerator*other.args[1]
			if denominator is None:
				denominator = other.args[0]
			else:
				denominator = Expr("mul", denominator, other.args[0])
		elif denominator is None:
			denominator = other
		else:
			denominator = Expr("mul", denominator, other)
		return Expr("div", numerator, denominator)
	
	def __mod__(self, other):
		if isinstance(other, Expr):
			other = other._unbox_single()
		return Expr("mod", self._unbox_single(), other)
	
	def __pow__(self, other):
		if self.op == "pow":
			return Expr("pow", self.args[0], Expr("mul", self.args[1], other))
		return Expr("pow", self, other)
	
	def __radd__(self, other):
		return Expr("add", other).__add__(self)
	
	def __rsub__(self, other):
		return Expr("add", other).__sub__(self)
	
	def __rmul__(self, other):
		return Expr("mul", other).__mul__(self)
	
	def __rtruediv__(self, other):
		return Expr("mul", other).__truediv__(self)
	
	def __rmod__(self, other):
		return Expr("mul", other).__mod__(self)
	
	def __rpow__(self, other):
		return Expr("mul", other).__pow__(self)
	
	def __neg__(self):
		if isinstance(self, Expr) and self.op == "mul":
			i = self.args.index(-1)
			if i != -1:
				if len(self.args) == 2:
					return self.args[-1-i]
				return Expr("mul", *self.args[:i], *self.args[i+1:])
			for i, a in enumerate(self.args):
				if isinstance(a, Number):
					return Expr("mul", -a, *self.args[:i], *self.args[i+1:])
		return Expr("mul", -1, self)
	
	def __pos__(self):
		return self
	
	def __floor__(self):
		return Expr("floor", self)
	
	def __ceil__(self):
		return Expr("ceil", self)
	
	@staticmethod
	def str_prec(expr, prec=20):
		if not isinstance(expr, Expr):
			return str(expr)
		if expr.op == "add":
			s = "+".join(Expr.str_prec(a, 4) for a in expr.args)
			return s if 4 <= prec else "(" + s + ")"
		if expr.op == "mul":
			s = "*".join(Expr.str_prec(a, 3) for a in expr.args)
			return s if 3 <= prec else "(" + s + ")"
		if expr.op == "div":
			s = Expr.str_prec(expr.args[0], 3) + "/" + Expr.str_prec(expr.args[1], 2)
			return s if 3 <= prec else "(" + s + ")"
		if expr.op == "pow":
			s = Expr.str_prec(expr.args[0], 1) + "**" + Expr.str_prec(expr.args[1], 2)
			return s if 2 <= prec else "(" + s + ")"
		if expr.op == "sum":
			return ("sum_{" + str(expr.args[0]) + "=" + str(expr.args[1]) + "}^{" +
				str(expr.args[2]) + "}{" + str(expr.args[3]) + "}")
		return expr.op + "(" + ",".join(map(str, expr.args)) + ")"
	
	def __str__(self):
		return Expr.str_prec(self)
	
	def __repr__(self):
		return "Expr(" + self.op + "," + ",".join(map(repr, self.args)) + ")"
	
	def __eq__(self, other):
		if not isinstance(other, Expr):
			return False
		if self.op != other.op or len(self.args) != len(other.args):
			return False
		return all(a == b for (a, b) in zip(self.args, other.args))
	
	def __hash__(self):
		return hash((self.op,) + self.args)
	
	@staticmethod
	def sum(var, a, b, expr):
		return Expr("sum", var, a, b, expr)
	
	@staticmethod
	def case(expr, indc):
		return Expr("case", expr, indc)
	
	@staticmethod
	def normalize(expr):
		if isinstance(expr, Expr):
			expr.args = tuple(map(Expr.normalize, expr.args))
			return expr._normalize()
		return expr
	
	def _distribute_mul(self):
		"""Convert a mul expr of adds into an add expr of muls"""
		terms = []
		sums = []
		for a in self.args:
			if isinstance(a, Expr) and a.op == "add":
				sums.append(a)
			else:
				terms.append(a)
		if sums:
			return Expr("add", *(Expr("mul", *terms, *p) for p in
				itertools.product(*(s.args for s in sums))))
		return self
	
	@staticmethod
	def _flatten_mul_single(a, ps, qs):
		if not isinstance(a, Expr):
			ps.append(p)
		elif a.op == "mul":
			ps += a.args
		else:
			p, q = a.args
			if isinstance(p, Expr) and p.op == "mul":
				ps += p.args
			else:
				ps.append(p)
			if isinstance(q, Expr) and q.op == "mul":
				qs += q.args
			else:
				qs.append(q)
	
	def _flatten_mul(self):
		"""Lift mul/div arguments of a mul/div expr"""
		if self.op == "mul":
			mixs = [i for (i, a) in enumerate(self.args) if
				isinstance(a, Expr) and (a.op == "mul" or a.op == "div")]
			if not mixs:
				return self
			ps = []
			qs = []
			ps += self.args[:mixs[0]]
			Expr._flatten_mul_single(self.args[mixs[0]], ps, qs)
			for i in range(1, len(mixs)):
				ps += self.args[mixs[i-1]+1:mixs[i]]
				Expr._flatten_mul_single(self.args[mixs[i]], ps, qs)
			ps += self.args[mixs[-1]+1:]
		else:
			p, q = self.args
			if not ((isinstance(p, Expr) and p.op == "div") or
					(isinstance(q, Expr) and q.op == "div")):
				return self
			Expr._flatten_mul_single(p, ps, qs)
			Expr._flatten_mul_single(q, qs, ps)
		if qs:
			p = Expr("mul", *ps) if len(ps) != 1 else ps[0]
			q = Expr("mul", *qs) if len(qs) != 1 else qs[0]
			return Expr("div", p, q)
		return Expr("mul", *ps)
	
	@staticmethod
	def _combine_mul_single(a, coeff, var_powers, expr_powers, in_denom):
		if isinstance(a, Number):
			coeff *= a
		elif isinstance(a, Var):
			var_powers[a.name] += -1 if in_denom else 1
		elif isinstance(a, Expr):
			if a.op == "pow" and isinstance(a.args[1], Number):
				if isinstance(a.args[0], Var):
					var_powers[a.args[0].name] += -a.args[1] if in_denom else a.args[1]
				elif isinstance(a.args[0], Expr):
					expr_powers[a.args[0]] += -a.args[1] if in_denom else a.args[1]
			else:
				expr_powers[a] += -1 if in_denom else 1
		return coeff
	
	@staticmethod
	def _unpack_powers(powmap, p_terms, q_terms):
		for k, v in powmap.items():
			v = Expr.normalize(v)
			if isinstance(k, str):
				k = Var(k)
			if not isinstance(v, Number):
				if (not isinstance(v, Expr) or v.op != "mul" or
						not isinstance(v.args[0], Number) or v.args[0] > 0):
					p_terms.append(Expr("pow", k, v))
				elif v.args[0] == -1:
					q_terms.append(Expr("pow", k, Expr("mul", *v.args[1:]).
						_unbox_single()))
				else:
					q_terms.append(Expr("pow", k, Expr("mul", -v.args[0], *v.args[1:]).
						_unbox_single()))
			elif v > 0:
				if v == 1:
					p_terms.append(k)
				else:
					p_terms.append(Expr("pow", k, v))
			elif v < 0:
				if v == -1:
					q_terms.append(k)
				else:
					q_terms.append(Expr("pow", k, -v))
	
	def _combine_mul(self):
		"""Convert self multiplications into powers"""
		var_powers = defaultdict(int)
		expr_powers = defaultdict(int)
		p_terms = []
		p_coeff = Q1
		q_terms = []
		q_coeff = Q1
		p, q = self.args if self.op == "div" else (self, None)
		if isinstance(p, Expr) and p.op == "mul":
			if p.op == "mul":
				for a in p.args:
					p_coeff = Expr._combine_mul_single(a, p_coeff, var_powers,
						expr_powers, False)
		else:
			p_coeff = Expr._combine_mul_single(p, p_coeff, var_powers,
				expr_powers, False)
		if q is not None:
			if isinstance(q, Expr) and q.op == "mul":
				for a in q.args:
					q_coeff = Expr._combine_mul_single(a, q_coeff, var_powers,
						expr_powers, True)
			else:
				q_coeff = Expr._combine_mul_single(q, q_coeff, var_powers,
					expr_powers, True)			
			p_coeff /= q_coeff
		Expr._unpack_powers(var_powers, p_terms, q_terms)
		Expr._unpack_powers(expr_powers, p_terms, q_terms)
		if p_coeff == 0:
			return Q0
		elif p_coeff == 1:
			p = Expr("mul", *p_terms)._unbox_single()
		else:
			p = Expr("mul", p_coeff, *p_terms)._unbox_single()
		if q_terms:
			q = Expr("mul", *q_terms)._unbox_single()
			return Expr("div", p, q) if q != 1 else p
		return p
	
	def _unbox_single(self):
		"""Replace singleton add or mul with its argument"""
		if self.op == "add" or self.op == "mul":
			if len(self.args) == 1:
				return self.args[0]
			elif len(self.args) == 0:
				return Q0 if self.op == "add" else Q1
		return self
	
	def _flatten_add(self):
		"""Lift add arguments of an add expr"""
		args = ()
		l = -1
		for i, a in enumerate(self.args):
			if isinstance(a, Expr) and a.op == "add":
				args = args + self.args[l+1:i] + a.args
				l = i
		if l != -1:
			return Expr("add", *args, *self.args[l+1:])
		return self
	
	@staticmethod
	def _combine_add_init_single(a, term_columns):
		if isinstance(a, Expr):
			if a.op == "mul":
				for aa in a.args:
					Expr._combine_add_init_single(aa, term_columns)
			elif a.op == "pow":
				term_columns.setdefault(a.args[0], len(term_columns))
			else:
				term_columns.setdefault(a, len(term_columns))
		elif isinstance(a, Var):
			term_columns.setdefault(a, len(term_columns))
	
	@staticmethod
	def _combine_add_addpow_single(a, term_columns, power):
		if isinstance(a, Expr):
			if a.op == "mul":
				for aa in a.args:
					Expr._combine_add_addpow_single(aa, term_columns, power)
			elif a.op == "pow":
				power[term_columns[a.args[0]]] = a.args[1]
			else:
				power[term_columns[a]] = 1
		elif isinstance(a, Var):
			power[term_columns[a]] = 1
	
	@staticmethod
	def _combine_add_getpow_single(a, term_columns):
		power = [0]*len(term_columns)
		Expr._combine_add_addpow_single(a, term_columns, power)
		return tuple(power)
	
	def _combine_add_collect(self, term_columns, coeffs):
		for a in self.args:
			#print("monomial:", a)
			power = Expr._combine_add_getpow_single(a, term_columns)
			#print("power:", power)
			if isinstance(a, Expr) and a.op == "mul" and isinstance(a.args[0], Number):
				coeff = a.args[0]
			elif isinstance(a, Number):
				coeff = a
			else:
				coeff = Q1
			coeffs[power] += coeff
	
	@staticmethod
	def _combine_add_monomial(coeff, power, terms):
		if coeff == 1:
			return Expr("mul", *(Expr("pow", terms[i], p)._flatten_pow() for (i, p) in
				enumerate(power) if p != 0))._unbox_single()
		return Expr("mul", coeff, *(Expr("pow", terms[i], p)._flatten_pow() for (i, p) in
			enumerate(power) if p != 0))._unbox_single()
	
	def _combine_add(self):
		"""Combine monomials with the same powers of the same variables and expressions"""
		#print("_combine_add called")
		term_columns = {}
		for a in self.args:
			Expr._combine_add_init_single(a, term_columns)
		#print("term_columns:")
		#for term, column in term_columns.items():
		#	print("  " + str(term) + ": " + str(column))
		coeffs = defaultdict(Q)
		self._combine_add_collect(term_columns, coeffs)
		#print("coeffs:")
		#for power, coeff in coeffs.items():
		#	print("  " + str(power) + ": " + str(coeff))
		terms = [None]*len(term_columns)
		for term, i in term_columns.items():
			terms[i] = term
		return Expr("add", *(Expr._combine_add_monomial(coeff, power, terms) for
			(power, coeff) in coeffs.items() if coeff != 0))._unbox_single()
	
	def _lcm_add(self):
		"""Combine divs within an add using lcm"""
		pass
	
	def _expand_pfrac(self):
		"""Expand a div into an add of divs using partial fractions"""
		pass
	
	def _flatten_pow(self):
		"""Convert repeated pows to one pow with a mul exponent"""
		if self.args[0] == 1 or (self.args[1] == 0 and self.args[0] != 0):
			return Q1
		if self.args[1] == 1:
			return self.args[0]
		if isinstance(self.args[0], Rational) and isinstance(self.args[1], Integral):
			return self.args[0]**self.args[1]
		if self.args[1] == -1:
			return Expr("div", Q1, self.args[0])
		if isinstance(self.args[1], Number) and self.args[1] < 0:
			return Expr("div", Q1, Expr("pow", self.args[0], -self.args[1]))
		if (isinstance(self.args[1], Expr) and self.args[1].op == "mul" and
				isinstance(self.args[1].args[0], Number) and self.args[1].args[0] < 0):
			if self.args[1].args[0] == -1:
				return Expr("div", Q1, Expr("mul", *self.args[1].args[1:]))
			return Expr("div", Q1, Expr("mul", -self.args[1].args[0],
				*self.args[1].args[1:]))
		if isinstance(self.args[0], Expr) and self.args[0].op == "pow":
			return Expr("pow", self.args[0].args[0], Expr("mul", self.args[0].args[1],
				self.args[1])._normalize())
		return self
	
	def _expand_multinomial(self):
		"""Expand multinomials with sufficiently few terms and low integer exponent"""
		pass
	
	def _distribute_pow(self):
		"""Convert a pow expr with a mul base to a mul expr where each term is a
		pow expr"""
		if isinstance(self.args[0], Expr):
			if self.args[0].op == "div":
				p, q = self.args[0].args
				return Expr("div", Expr("pow", p, self.args[1])._distribute_pow(),
					Expr("pow", q, self.args[1])._distribute_pow())
			elif self.args[0].op == "mul":
				return Expr("mul", *(Expr("pow", a, self.args[1])._flatten_pow() for a in
					self.args[0].args))
		return self
	
	def _normalize(self):
		if self.op == "mul":
			expr = self._distribute_mul()
			if expr.op == "mul":
				expr = expr._flatten_mul()._combine_mul()
				return expr._unbox_single() if isinstance(expr, Expr) else expr
			else:
				expr.args = tuple(a._normalize() for a in expr.args)
				return expr._normalize()
		if self.op == "div":
			expr = self._flatten_mul()._combine_mul()
			return expr._unbox_single() if isinstance(expr, Expr) else expr
		elif self.op == "add":
			expr = self._flatten_add()._combine_add()
			return expr._unbox_single() if isinstance(expr, Expr) else expr
		elif self.op == "pow":
			expr = self._flatten_pow()
			if expr.op == "pow":
				#expr._expand_multinomial()
				return expr._distribute_pow()
			return expr._normalize()
		elif self.op == "floor":
			pass
		elif self.op == "ceil":
			pass
		elif self.op == "case":
			pass
		elif self.op == "sum":
			pass
		return self
	
	def _sum_faulhaber(self):
		"""Symbolically compute sums of polynomials"""
		if self.op != "sum":
			raise TypeError("Expression is not a sum")
		idx, a, b, expr = self.args
		"""Overview of summation: sum(idx, a, b, x + y*z) =
		x*sum(idx, a, b, 1) + y*sum(idx, a, b, z) where x, y do not depend on idx
		but z does.  Then we have the faulhaber rule which says sum(n**p) =
		sum(j, 0, p, binom(p+1, j)*bernoulli(j)*n**(p+1-j))/(p+1).
		The diffeogeometric rule gives us sum(P(n)*r**n) for any polynomial P
		and number r.
		Finally, the case rules tell us floor(p/q) =
		case(p/q, p%q) + case((p-1)/q, (p-1)%q) + ... and
		sum(k, 0, n, case((k-p)/q, (k-p)%q)) =
		sum(k, 0, floor(n/q), (qk-p)/q),
		give or take."""
	
	def _sum_diffeogeometric(self):
		"""Symbolically compute sums of polynomials times exponentials"""
		pass
	
	def _sum_floorceil(self):
		"""Convert a sum involving floor or ceil of division by an integer
		to one sum for each residue class and cases"""
		pass

