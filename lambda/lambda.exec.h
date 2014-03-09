
#ifndef __lambda__lambda_exec__
#define __lambda__lambda_exec__

#include "lambda.sexpr.util.h"

// Note:	execution was a complete fail the first time
//			we must execute forward and lazy.
//
//			eg. (+ 4 5 7 9 (- 3 5)) => (+ 25 (- 3 5))
//
//			not: (+ 4 5 7 9 (- 3 5)) => (+ 4 5 7 9 -2)
//
//			This has the added benefit of being lazy (and able to work
//			with arbitrary list sizes.
//
//			ie. eval is called during execution.
//
//			This does not stop us from pre-computing stuff either.
//
//			Also better if it makes lambda functions more natural
//
//
//	Note2:	Execution must handle infinite types.  Which makes things
//			more interesting.
//
//			(+ 4 5 (- 3 4 5 6)) => (+9 + 3 (- _ 4 5 6))
//
//			The tail-end is more of a generator.

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
