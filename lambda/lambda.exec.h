
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
//
//
//	Note3:	Upon reading the standard methods to deal with
//			executing functional languages; it has come to my
//			attention that the data structure is not ideal.
//
//			Missing is the dump (stack of stacks) to hold
//			the evaluations.
//
//			Also; much of the logic comes out of data structures,
//			note the other way around.  Lists are built up from the
//			language -- not internal to.
//
//			Left-tree is operations, right are values.
//
//			We need to build an appropriate tree from the given
//			S-Expressions.
//
//				*  <-- partial application of 4  :D
//			   / \
//			  /   \
//			 +     4
//
//			Lazy evaluation also arises if we use this structure
//			properly.
//
//
//	Note4:	Storing a list of instructions is not bad.
//			push x, push y, eval p
//
//			This is equivalent to the tree though...

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
