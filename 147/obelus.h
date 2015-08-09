#ifndef __OBELUS_H__
#define __OBELUS_H__

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <hac_vec.h>
#include <hac_avl.h>

typedef struct OBL_Expr OBL_Expr;

HAC_VEC_T_NEW(OBL_Expr);

typedef uint64_t OBL_Val;
typedef const char *OBL_Var;
typedef HAC_VEC_T(OBL_Expr) OBL_Add;
typedef HAC_VEC_T(OBL_Expr) OBL_Mul;
typedef struct{OBL_Expr *num, *denom;} OBL_Div;
typedef struct{OBL_Expr *base, *exp;} OBL_Exp;
typedef struct{const char *name; HAC_VEC_T(OBL_Expr) args;} OBL_Fun;

HAC_AVL_T_NEW(OBL_Var);

struct OBL_Expr{
	enum{OBL_VAL, OBL_VAR, OBL_ADD, OBL_MUL, OBL_DIV, OBL_EXP, OBL_FUN, OBL_BAD} type;
	HAC_AVL_T(OBL_Var) vs;//TODO: Reduce space taken by variable sets by keeping a cache.
	union{
		OBL_Val Val;
		OBL_Var Var;
		OBL_Add Add;
		OBL_Mul Mul;
		OBL_Div Div;
		OBL_Exp Exp;
		OBL_Fun Fun;
	};
};

OBL_Expr OBL_val(OBL_Val v);

OBL_Expr OBL_var(OBL_Var n);

OBL_Expr __OBL_add(HAC_VEC_T(OBL_Expr) ts);

#define OBL_add(ts...) __OBL_add(HAC_VEC_LIST(OBL_Expr, ts...))

OBL_Expr __OBL_mul(HAC_VEC_T(OBL_Expr) ts);

#define OBL_mul(ts...) __OBL_mul(HAC_VEC_LIST(OBL_Expr, ts...))

OBL_Expr __OBL_div(OBL_Expr *num, OBL_Expr *denom);

OBL_Expr OBL_div(OBL_Expr num, OBL_Expr denom);

OBL_Expr __OBL_exp(OBL_Expr *base, OBL_Expr *exp);

OBL_Expr OBL_exp(OBL_Expr base, OBL_Expr exp);

OBL_Expr __OBL_fun(const char *name, HAC_VEC_T(OBL_Expr) args);

#define fun(name, args...) _fun(name, HAC_VEC_LIST(OBL_Expr, args...))

OBL_Expr OBL_Bad(void);

void OBL_DeleteExpr(OBL_Expr *x);

void OBL_Expand(OBL_Expr *x);

#endif

