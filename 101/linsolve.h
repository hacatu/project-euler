#pragma once
#include <stdlib.h>
#include <gmp.h>

int linsolve(size_t nxs, mpq_t **eqns, mpq_t *xs);

