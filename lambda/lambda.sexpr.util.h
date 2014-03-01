
#ifndef __lambda__lambda_sexpr_util__
#define __lambda__lambda_sexpr_util__

#include "lambda.sexpr.h"
#include <functional>

namespace lambda
{
	void walkSexpr(Sexpr &root,
				   std::function<void (Sexpr &n, int depth)> fn,
				   std::function<void (Sexpr &child, int depth)> newDepthFn,
				   int depth=0);
	
	void debugPrint(Sexpr &root);
}

#endif
