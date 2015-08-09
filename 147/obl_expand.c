#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <hac_vec.h>
#include <hac_avl.h>
#include "obelus.h"

static void OBL_ExpandAdd(OBL_Expr *x);
static void OBL_ExpandMul(OBL_Expr *x);
static void OBL_ExpandDiv(OBL_Expr *x);
static void OBL_ExpandExp(OBL_Expr *x);
static void OBL_ExpandFun(OBL_Expr *x);

void OBL_Expand(OBL_Expr *x){
	switch(x->type){
		case OBL_VAL:
		break;
		case OBL_VAR:
		break;
		case OBL_ADD:
		OBL_ExpandAdd(x);
		break;
		case OBL_MUL:
		OBL_ExpandMul(x);
		break;
		case OBL_DIV:
		OBL_ExpandDiv(x);
		break;
		case OBL_EXP:
		OBL_ExpandExp(x);
		break;
		case OBL_FUN:
		OBL_ExpandFun(x);
		break;
		case OBL_BAD:
		break;
	}
}

void OBL_ExpandAdd(OBL_Expr *x){
	HAC_VEC_FOREACHP(OBL_Expr, &x->Add, OBL_Expand(e));//Expand all terms being added
	size_t ts = HAC_VEC_FOLDR(OBL_Expr, &x->Add, (size_t)0, e.type == OBL_ADD ? l + e.Add.n : l + 1);//If there are any OBL_ADD terms, these can be lifted into the current Add level
	HAC_VEC_RESIZE(OBL_Expr, &x->Add, ts);
	ts = x->Add.n;
	for(size_t i = 0; i < ts; ++i){
		if(x->Add.a[i].type != OBL_ADD){
			continue;
		}
		memcpy(x->Add.a + x->Add.n, x->Add.a[i].Add.a + 1, (x->Add.a[i].Add.n - 1)*sizeof(OBL_Expr));
		x->Add.n += x->Add.a[i].Add.n - 1;
		x->Add.a[i] = x->Add.a[i].Add.a[0];
		HAC_AVL_DELETE(OBL_Var, &x->Add.a[i].vs);
		HAC_VEC_DELETE(OBL_Expr, &x->Add.a[i].Add);//TODO: Fix this so it deletes the vector.
	}//Done lifting
	size_t fs = HAC_VEC_FOLDR(OBL_Expr, &x->Add, (size_t)0, e.type == OBL_DIV ? l + 1 : l);//If there are any fraction terms being added, they have to be added using lcm
	if(!fs){
		return;
	}
	HAC_VEC_T(OBL_Expr) num = HAC_VEC_NEW(OBL_Expr, x->Add.n), t;
	for(size_t i = 0; i < x->Add.n; ++i){//TODO: Replace this loop with a proper gcd calculation, or at least reduce recalculation.
		t = HAC_VEC_NEW(OBL_Expr, x->Add.a[i].type == OBL_DIV ? fs - 1 : fs);
		HAC_VEC_PUSHR(OBL_Expr, &t, x->Add.a[i].type == OBL_DIV ? *x->Add.a[i].Div.num : x->Add.a[i]);
		for(size_t j = 0; j < x->Add.n; ++j){
			if(j == i || x->Add.a[j].type != OBL_DIV){
				continue;
			}
			HAC_VEC_PUSHR(OBL_Expr, &t, *x->Add.a[j].Div.denom);
		}
		num.a[i] = __OBL_mul(t);
	}
	t = HAC_VEC_NEW(OBL_Expr, fs);
	for(size_t i = 0; i < x->Add.n; ++i){
		if(x->Add.a[i].type == OBL_DIV){
			HAC_VEC_PUSHR(OBL_Expr, &t, *x->Add.a[i].Div.denom);
		}
	}
	HAC_VEC_DELETE(OBL_Expr, &x->Add);
	x->type = OBL_DIV;
	x->Div.num = malloc(1*sizeof(OBL_Expr));
	if(!x->Div.num){
		x->type = OBL_BAD;
		return;
	}
	x->Div.denom = malloc(1*sizeof(OBL_Expr));
	if(!x->Div.denom){
		x->type = OBL_BAD;
		return;
	}
	*x->Div.num = (OBL_Expr){.type = OBL_ADD, .Add = num, .vs = x->vs};
	*x->Div.denom = (OBL_Expr){.type = OBL_MUL, .Mul = t, .vs = x->vs};//Done adding fraction terms.
	OBL_ExpandAdd(x->Div.num);
	OBL_ExpandMul(x->Div.denom);
}

static int __OBL_cmpF(OBL_Expr a, OBL_Expr b){
	switch(a.type){
		case OBL_VAL:
		return 1;
		break;
		case OBL_VAR:
		if(b.type == OBL_VAL){
			return -1;
		}else if(b.type == OBL_VAR){
			return strcmp(a.Var, b.Var);
		}else if(b.type == OBL_EXP && b.Exp.base->type == OBL_VAR){
			return strcmp(a.Var, b.Exp.base->Var);
		}
		return 1;
		break;
		case OBL_EXP:
		if(a.Exp.base->type == OBL_VAR){
			if(b.type == OBL_VAL){
				return -1;
			}else if(b.type == OBL_VAR){
				return strcmp(a.Exp.base->Var, b.Var);
			}else if(b.type == OBL_EXP && b.Exp.base->type == OBL_VAR){
				return strcmp(a.Exp.base->Var, b.Exp.base->Var);
			}
			return 1;
		}else{
			if(b.type == OBL_VAL){
				return -1;
			}else if(b.type == OBL_VAR){
				return -1;
			}else if(b.type == OBL_EXP){
				if(b.Exp.base->type == OBL_VAR){
					return -1;
				}
				return strcmp(a.Exp.base->Fun.name, b.Exp.base->Fun.name);
			}
			return strcmp(a.Exp.base->Fun.name, b.Fun.name);
		}
		break;
		case OBL_FUN:
		if(b.type == OBL_VAL){
			return -1;
		}else if(b.type == OBL_VAR){
			return -1;
		}else if(b.type == OBL_EXP){
			if(b.Exp.base->type == OBL_VAR){
				return -1;
			}
			return strcmp(a.Fun.name, b.Exp.base->Fun.name);
		}
		return strcmp(a.Fun.name, b.Fun.name);
		default:
		return 0;
	}
}

static void __OBL_CombineMulVar(OBL_Expr *x, HAC_VEC_T(OBL_Expr) *prod, size_t *i){
	size_t j = *i + 1;
	while(j <= x->Mul.n && !__OBL_cmpF(x->Mul.a[*i], x->Mul.a[j])){
		++j;
	}
	if(j - *i == 1 && x->Mul.a[*i].type != OBL_EXP){
		HAC_VEC_PUSHR(OBL_Expr, prod, x->Mul.a[*i]);
		*i = j;
		return;
	}
	HAC_VEC_T(OBL_Expr) sum = HAC_VEC_NEW(OBL_Expr, j - *i);
	for(size_t k = *i; k < j; ++k){
		if(x->Mul.a[k].type == OBL_EXP){
			HAC_VEC_PUSHR(OBL_Expr, &sum, *x->Mul.a[k].Exp.exp);
		}else{
			HAC_VEC_PUSHR(OBL_Expr, &sum, OBL_val(1));
		}
	}
	OBL_Expr power = OBL_exp(x->Mul.a[*i].type == OBL_EXP ? *x->Mul.a[*i].Exp.base : x->Mul.a[*i], __OBL_add(sum));
	OBL_ExpandAdd(power.Exp.exp);
	HAC_VEC_PUSHR(OBL_Expr, prod, power);
	*i = j;
}

static void __OBL_CombineMulFun(OBL_Expr *x, HAC_VEC_T(OBL_Expr) *prod, size_t *i){
	__OBL_CombineMulVar(x, prod, i);
}

static void __OBL_CombineMul(OBL_Expr *x){
	HAC_VEC_T(OBL_Expr) prod = HAC_VEC_NEW(OBL_Expr, x->Mul.n);
	size_t i = 0;
	for(; i <= x->Mul.n && x->Mul.a[i].type == OBL_VAL; ++i){}
	if(i){
		OBL_Val p = 1;
		for(size_t j = 0; j < i; ++j){
			p *= x->Mul.a[j].Val;
		}
		HAC_VEC_PUSHR(OBL_Expr, &prod, OBL_val(p));
	}
	while(i < x->Mul.n && (x->Mul.a[i].type == OBL_VAR || (x->Mul.a[i].type == OBL_EXP && x->Mul.a[i].Exp.base->type == OBL_VAR))){
		__OBL_CombineMulVar(x, &prod, &i);
	}
	while(i < x->Mul.n && (x->Mul.a[i].type == OBL_FUN || (x->Mul.a[i].type == OBL_EXP && x->Mul.a[i].Exp.base->type == OBL_FUN))){
		__OBL_CombineMulFun(x, &prod, &i);
	}
	HAC_VEC_TRIM(OBL_Expr, &prod);
	x->Mul = prod;
}

static void __OBL_ExpandMul(OBL_Expr *x){
	//First, check if there are any additions bieng multiplied, then put them bae.
	OBL_Expr t;
	size_t ts = 1, a = 0;
	for(size_t b = x->Mul.n - 1; a <= b;){
		if(x->Mul.a[a].type != OBL_ADD){
			++a;
			continue;
		}
		if(x->Mul.a[b].type == OBL_ADD){
			ts *= x->Mul.a[b].Add.n;
			--b;
			continue;
		}
		t = x->Mul.a[a];
		x->Mul.a[a] = x->Mul.a[b];
		x->Mul.a[b] = t;
	}
	if(ts == 1){//THere are no additions.  Put the terms in the order vals, vars and powers in alphabetical order, and functions.
		//HAC_VEC_SORT(OBL_Expr, &x->Mul, __OBL_cmpF(e1, e2), HAC_VEC_SWAP(OBL_Expr, v, i, j));
		({
			HAC_VEC_T(OBL_Expr) *_vec = (&x->Mul);
			__HAC_VEC_QSORT(OBL_Expr, _vec, 0, _vec->n, __OBL_cmpF(e1, e2), HAC_VEC_SWAP(OBL_Expr, v, i, j)));
		});
		__OBL_CombineMul(x);
		return;
	}
	size_t cmb[a];
	for(size_t i = 0; i < a; ++i){
		cmb[i] = 0;
	}
	HAC_VEC_T(OBL_Expr) sum = HAC_VEC_NEW(OBL_Expr, ts), prod;
	while(1){
		prod = HAC_VEC_NEW(OBL_Expr, a);
		for(size_t i = 0; i < a; ++i){
			HAC_VEC_PUSHR(OBL_Expr, &prod, x->Mul.a[i].Add.a[cmb[i]]);//TODO: Copy the elements so this actually works.
		}
		t = __OBL_mul(prod);
		HAC_VEC_PUSHR(OBL_Expr, &sum, t);
		size_t j = 0;
		while(cmb[j] == x->Mul.a[j].Add.n - 1 && j < a){
			++j;
		}
		if(j == a){
			break;
		}
		++cmb[j];
	}
	*x = __OBL_add(sum);
}

void OBL_ExpandMul(OBL_Expr *x){
	HAC_VEC_FOREACHP(OBL_Expr, &x->Mul, OBL_Expand(e));
	size_t ts = HAC_VEC_FOLDR(OBL_Expr, &x->Mul, (size_t)0, e.type == OBL_MUL ? l + e.Mul.n : l + 1);
	HAC_VEC_RESIZE(OBL_Expr, &x->Mul, ts);
	ts = x->Mul.n;
	for(size_t i = 0; i < ts; ++i){
		if(x->Mul.a[i].type != OBL_MUL){
			continue;
		}
		memcpy(x->Mul.a + x->Mul.n, x->Mul.a[i].Add.a + 1, (x->Mul.a[i].Mul.n - 1)*sizeof(OBL_Expr));
		x->Mul.n += x->Mul.a[i].Mul.n - 1;
		x->Mul.a[i] = x->Mul.a[i].Mul.a[0];
		HAC_AVL_DELETE(OBL_Var, &x->Mul.a[i].vs);
		HAC_VEC_DELETE(OBL_Expr, &x->Mul.a[i].Mul);
	}
	size_t fs = HAC_VEC_FOLDR(OBL_Expr, &x->Mul, (size_t)0, e.type == OBL_DIV ? l + 1 : l);
	if(!fs){
		__OBL_ExpandMul(x);
		return;
	}
	HAC_VEC_T(OBL_Expr) num = HAC_VEC_MAP(OBL_Expr, &x->Mul, e.type == OBL_DIV ? *e.Div.num : e);
	HAC_VEC_T(OBL_Expr) denom = HAC_VEC_NEW(OBL_Expr, fs);
	HAC_VEC_FOREACH(OBL_Expr, &x->Mul, ({
		if(e.type == OBL_DIV){
			HAC_VEC_PUSHR(OBL_Expr, &denom, *e.Div.denom);
		}
	}));
	HAC_VEC_DELETE(OBL_Expr, &x->Mul);
	*x = OBL_div(__OBL_mul(num), __OBL_mul(denom));
}

