
#ifndef __lambda__lambda_exec__
#define __lambda__lambda_exec__

#include "lambda.sexpr.util.h"

namespace lambda
{
	class exec
	{
	public:
		exec(Sexpr in_expr);
		
		Sexpr eval();
		
	private:
		Sexpr _expr;
	};
	typedef std::shared_ptr<exec> Exec;
}

#endif
