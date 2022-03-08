//fla.h
#ifndef __FLA_H__
#define __FLA_H__

#include <stddef.h>

#define FLA_CAST(T, p) (((union{__typeof__(p) data; T a;})(p)).a)

#endif

