#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <hac_vec.h>
#include <hac_avl.h>
#include "obelus.h"

OBL_Expr __OBL_add(HAC_VEC_T(OBL_Expr) ts){
	OBL_Expr ret;
	ret.type = OBL_ADD;
	ret.Add = ts;
	ret.vs = HAC_AVL_EMPTY(OBL_Var);//TODO: manage vs in a separate function to enable constructing expressions easily with known variable signatures.
	HAC_VEC_FOREACH(OBL_Expr, &ts, ({
		for(__HAC_AVL_NODE_T(OBL_Var) *node = __HAC_AVL_FIRST_PREORDER(OBL_Var, &e.vs); node; node = __HAC_AVL_NEXT_PREORDER(OBL_Var, node)){
			HAC_AVL_INSERT(OBL_Var, &ret.vs, node->v, strcmp(e1, e2));
		}
	}));
	return ret;
}

#define OBL_add(ts...) __OBL_add(HAC_VEC_LIST(OBL_Expr, ts...))

OBL_Expr __OBL_mul(HAC_VEC_T(OBL_Expr) ts){
	OBL_Expr ret;
	ret.type = OBL_MUL;
	ret.Mul = ts;
	ret.vs = HAC_AVL_EMPTY(OBL_Var);
	HAC_VEC_FOREACH(OBL_Expr, &ts, ({
		for(__HAC_AVL_NODE_T(OBL_Var) *node = __HAC_AVL_FIRST_PREORDER(OBL_Var, &e.vs); node; node = __HAC_AVL_NEXT_PREORDER(OBL_Var, node)){
			HAC_AVL_INSERT(OBL_Var, &ret.vs, node->v, strcmp(e1, e2));
		}
	}));
	return ret;
}

#define OBL_mul(ts...) __OBL_mul(HAC_VEC_LIST(OBL_Expr, ts...))

OBL_Expr __OBL_div(OBL_Expr *num, OBL_Expr *denom){
	OBL_Expr ret;
	ret.type = OBL_DIV;
	ret.Div = (OBL_Div){.num = num, .denom = denom};
	ret.vs = HAC_AVL_COPY(OBL_Var, &num->vs);
	for(__HAC_AVL_NODE_T(OBL_Var) *node = __HAC_AVL_FIRST_PREORDER(OBL_Var, &denom->vs); node; node = __HAC_AVL_NEXT_PREORDER(OBL_Var, node)){
		HAC_AVL_INSERT(OBL_Var, &ret.vs, node->v, strcmp(e1, e2));
	}
	return ret;
}

OBL_Expr OBL_div(OBL_Expr num, OBL_Expr denom){
	OBL_Expr *_num, *_denom;
	_num = malloc(1*sizeof(OBL_Expr));
	if(!_num){
		return OBL_Bad();
	}
	_denom = malloc(1*sizeof(OBL_Expr));
	if(!_denom){
		free(_num);
		return OBL_Bad();
	}
	*_num = (num);
	*_denom = (denom);
	return __OBL_div(_num, _denom);
}

OBL_Expr __OBL_exp(OBL_Expr *base, OBL_Expr *exp){
	OBL_Expr ret;
	ret.type = OBL_EXP;
	ret.Exp = (OBL_Exp){.base = base, .exp = exp};
	ret.vs = HAC_AVL_COPY(OBL_Var, &base->vs);
	for(__HAC_AVL_NODE_T(OBL_Var) *node = __HAC_AVL_FIRST_PREORDER(OBL_Var, &exp->vs); node; node = __HAC_AVL_NEXT_PREORDER(OBL_Var, node)){
		HAC_AVL_INSERT(OBL_Var, &ret.vs, node->v, strcmp(e1, e2));
	}
	return ret;
}

OBL_Expr OBL_exp(OBL_Expr base, OBL_Expr exp){
	OBL_Expr *_base, *_exp;
	_base = malloc(1*sizeof(OBL_Expr));
	if(!_base){
		return OBL_Bad();
	}
	_exp = malloc(1*sizeof(OBL_Expr));
	if(!_exp){
		free(_base);
		return OBL_Bad();
	}
	*_base = (base);
	*_exp = (exp);
	return __OBL_div(_base, _exp);
}

OBL_Expr __OBL_fun(const char *name, HAC_VEC_T(OBL_Expr) args){
	OBL_Expr ret;
	ret.type = OBL_FUN;
	ret.Fun = (OBL_Fun){.name = name, .args = args};
	ret.vs = HAC_AVL_EMPTY(OBL_Var);
	HAC_VEC_FOREACH(OBL_Expr, &args, ({
		for(__HAC_AVL_NODE_T(OBL_Var) *node = __HAC_AVL_FIRST_PREORDER(OBL_Var, &e.vs); node; node = __HAC_AVL_NEXT_PREORDER(OBL_Var, node)){
			HAC_AVL_INSERT(OBL_Var, &ret.vs, node->v, strcmp(e1, e2));
		}
	}));
	return ret;
}

#define fun(name, args...) _fun(name, HAC_VEC_LIST(OBL_Expr, args...))

OBL_Expr OBL_Bad(void){
	return (OBL_Expr){.type = OBL_BAD};
}

void OBL_DeleteExpr(OBL_Expr *x){
	switch(x->type){
		case OBL_VAL:
		break;
		case OBL_VAR:
		break;
		case OBL_ADD:
		HAC_VEC_FOREACHP(OBL_Expr, &x->Add, OBL_DeleteExpr(e));
		HAC_VEC_DELETE(OBL_Expr, &x->Add);
		break;
		case OBL_MUL:
		HAC_VEC_FOREACHP(OBL_Expr, &x->Mul, OBL_DeleteExpr(e));
		HAC_VEC_DELETE(OBL_Expr, &x->Mul);
		break;
		case OBL_DIV:
		OBL_DeleteExpr(x->Div.num);
		OBL_DeleteExpr(x->Div.denom);
		free(x->Div.num);
		free(x->Div.denom);
		break;
		case OBL_EXP:
		OBL_DeleteExpr(x->Exp.base);
		OBL_DeleteExpr(x->Exp.exp);
		free(x->Exp.base);
		free(x->Exp.exp);
		break;
		case OBL_FUN:
		HAC_VEC_FOREACHP(OBL_Expr, &x->Fun.args, OBL_DeleteExpr(e));
		HAC_VEC_DELETE(OBL_Expr, &x->Fun.args);
		break;
		case OBL_BAD:
		break;
	}
	if(x->type != OBL_BAD){
		HAC_AVL_DELETE(OBL_Var, &x->vs);
		x->type = OBL_BAD;
	}
}

