
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
	
	class sexpr
	{
	private:
		
	public:
		Sexpr next;
		
		virtual Sexpr child() { return NULL; }
		virtual String stringValue() { return String(new std::string("(empty)")); }
		virtual Sexpr eval()
		{
			Sexpr e = Sexpr(new sexpr());
			if (next)
				e->next = next->eval();
			return e;
		}
		
		Sexpr type;
		
		virtual Sexpr shallowCopy() { return Sexpr(new sexpr()); }
		virtual float floatValue() { return 0; }
	};
	
	
	class sexprExpr : public sexpr
	{
	public:
		
		sexprExpr(Sexpr in_child) : _child(in_child) {}
		virtual Sexpr child() { return _child; }
		virtual String stringValue()
		{
			if (_fnName == NULL)
				return String(new std::string("(child)"));
			else
			{
				std::string combo = "(function " + *_fnName + ")";
				return String(new std::string(combo));
			}
		}
		
		virtual Sexpr eval()
		{
			Sexpr r;
			
			if (_evaluator)
				r = _evaluator(_child->eval());
			else
				r = _child->eval();
				
			if (next)
				r ->next = next->eval();
			return r;
		}
		
		virtual Sexpr shallowCopy() { return Sexpr (new sexprExpr(_child));}
		
		Sexpr _child;
		String _fnName;
		std::function<Sexpr (Sexpr)> _evaluator;
	};
	
	
	typedef std::function<Sexpr (Sexpr)> fnSexpr;
	
	
	class sexprSymbol : public sexpr
	{
	public:
		sexprSymbol(String in_s) : symbol(in_s) {}
		virtual String stringValue() { return symbol; }
		
		virtual Sexpr eval()
		{
			Sexpr e = Sexpr(new sexprSymbol(symbol));
			if (next)
				e->next = next->eval();
			return e;
		}
		
		virtual Sexpr shallowCopy() { return Sexpr (new sexprSymbol(symbol));}
		
		String symbol;
	};
	
	
	class sexprInteger : public sexpr
	{
	public:
		sexprInteger(int in_i) : i(in_i) {}
		virtual String stringValue()
		{
			char tmp[16];
			snprintf(tmp, 16, "%i", i);
			return String(new std::string(tmp));
		}
		
		virtual Sexpr eval()
		{
			Sexpr e = Sexpr(new sexprInteger(i));
			if (next)
				e->next = next->eval();
			return e;
		}
		
		virtual float floatValue() { return (float)i; }
		
		virtual Sexpr shallowCopy() { return Sexpr (new sexprInteger(i));}
		
		int i;
	};
	
	
	class sexprFloat : public sexpr
	{
	public:
		sexprFloat(float in_f) : f(in_f) {}
		virtual String stringValue()
		{
			char tmp[16];
			snprintf(tmp, 16, "%f", f);
			return String(new std::string(tmp));
		}
		
		virtual Sexpr eval()
		{
			Sexpr e = Sexpr(new sexprFloat(f));
			if (next)
				e->next = next->eval();
			return e;
		}
		
		virtual float floatValue() { return f; }
		
		virtual Sexpr shallowCopy() { return Sexpr (new sexprFloat(f));}
		
		float f;
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
