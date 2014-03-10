
#ifndef __lambda__lambda_sexpr__
#define __lambda__lambda_sexpr__

#include "lambda.scanner.h"
#include <sstream>
#include <functional>

namespace lambda
{
	// Excessive number of objects makes it difficult to keep track of what's
	// happening.
	//
	// Much is, actually, duplicated.  That is why the execution engine now
	// treats everything identically.
	//
	// For example, we may have (Int Int Int).  This is a tuple.  Nothing more.
	//
	// Furthermore, (Int + Int) will match (Int + Int) => Int, thus evaluate
	// to an Int.  This is closer to Haskell, and allows keeping the execution
	// engine simpler.
	//
	// Type is determined at the lexical analysis stage.
	//
	
	class sexpr;
	typedef std::shared_ptr<sexpr> Sexpr;
	
	enum SexprType
	{
		S_NIL,
		S_SYMBOL,
		S_FLOAT32,		//!< Float 32 parsed + pushed onto stack
		S_INTEGER32,	//!< Int 32 parsed + pushed onto stack
		
		S_EXEC,			//!< Exec hardware fn (has ptr, int32 pops)
		
		S_CLOSURE,		//!< Basic closure
		
		S_EVAL,			//!< Evaluate a closure
		
		S_PARAM,		//!< Parameter
		S_SLIDE			//!< Slides a parameter down
	};
	
	class sexpr
	{
	private:
		
	public:
		sexpr(SexprType _type) : type(_type) {}
		sexpr(SexprType _type, Sexpr _child) : type(_type), child(_child) {}
		sexpr(SexprType _type, const char *_c)
		{
			type = _type;
			value.symbol = getStringPtr(_c);
		}
		sexpr(SexprType _type, int _i)
		{
			type = _type;
			value.i = _i;
		}
		sexpr(SexprType _type, float _f)
		{
			type = _type;
			value.f = _f;
		}
		
		Sexpr duplicate()
		{
			Sexpr r(new sexpr(type));
			r->next = next;
			r->child = child;
			r->eval = eval;
			r->value = value;
			return r;
		}
		
		Sexpr next;		//!< Next element
		Sexpr child;	//!< Child
		
		SexprType type;	//!< Type of data
		
		std::function<Sexpr (Sexpr, Sexpr)> eval; //!< Evaluation helper
		
		const std::string stringValue() const;
		
		union
		{
			int i;
			float f;
			const char *symbol;		//!< Note - uses shared ptr pool
		} value;
	};
	
	
	class lexer
	{
	public:
		lexer(Scanner &s);
		
		Sexpr lex();
		
	private:
		Sexpr expression();
		
		void read();
		
		Scanner _s;
		Token	_t;
	};
	typedef std::shared_ptr<lexer> Lexer;
}

#endif
