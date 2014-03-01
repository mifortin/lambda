
#ifndef __lambda__lambda_sexpr__
#define __lambda__lambda_sexpr__

#include "lambda.scanner.h"
#include <sstream>
#include <functional>

namespace lambda
{
	class sexpr;
	typedef std::shared_ptr<sexpr> Sexpr;
	
	class sexpr
	{
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
		virtual float floatValue() { return 0; }
	};
	
	
	class sexprExpr : public sexpr
	{
	public:
		sexprExpr(Sexpr in_child) : _child(in_child) {}
		virtual Sexpr child() { return _child; }
		virtual String stringValue() { return String(new std::string("(child)")); }
		
		virtual Sexpr eval()
		{
			Sexpr e = _child->eval();
			if (e->next)
				e = Sexpr(new sexprExpr(e));
			if (next)
				e->next = next->eval();
			return e;
		}
		
		Sexpr _child;
	};
	
	
	typedef std::function<Sexpr (Sexpr)> fnSexpr;
	
	
	class sexprEval : public sexpr
	{
	public:
		sexprEval(String name, fnSexpr eval)
		: _fnName(name), _evaluator(eval) {}
		
		virtual String stringValue()
		{
			char tmp[256];
			snprintf(tmp, 256, "(function %s)", _fnName->c_str());
			return String(new std::string(tmp));
		}
		
		virtual Sexpr eval()
		{
			return _evaluator(next->eval());
		}
		
		// Takes an SExpression and returns the evaluation of the SExpression
		String _fnName;
		std::function<Sexpr (Sexpr)> _evaluator;
	};
	
	
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
