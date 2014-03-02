
#include "lambda.exec.h"

namespace lambda
{
	static Sexpr TypeInteger(new sexprSymbol(getString("Int32")));
	static Sexpr TypeFloat(new sexprSymbol(getString("Float32")));
	
	template<class O>
	static Sexpr fnFloatSexpr(Sexpr e)
	{
		float f = 0;
		
		if (e)
		{
			f = e->floatValue();
			e = e->next;
		}
		
		while (e)
		{
			f = O::op(f, e->floatValue());
			e = e->next;
		}
		
		return Sexpr(new sexprFloat(f));
	}
	
	template<class T>
	struct OpPlus
	{
		static T op(T a, T b) { return a+b; }
	};
	
	template<class T>
	struct OpMinus
	{
		static T op(T a, T b) { return a-b; }
	};
	
	template<class T>
	struct OpMul
	{
		static T op(T a, T b) { return a*b; }
	};
	
	
	static void arithmeticCast(std::string b, sexprExpr* &s)
	{
		Sexpr sn = s->child();
		bool isInt = sn->type == TypeInteger;
		while (isInt && sn->next)
		{
			sn = sn->next;
			isInt = isInt && (sn->type == TypeInteger);
		}
		
		if (isInt)
		{
			s->_fnName = getString((b + "::Int32").c_str());
			s->type = TypeInteger;
		}
		else
		{
			s->_fnName = getString((b + "::Float32").c_str());
			s->type = TypeFloat;
		}
	}
	
	
	static std::map<std::string, std::function<void (sexprExpr*)>> builtin
	= {
		{"+", [] (sexprExpr* e) {
			arithmeticCast("+",e);
			e->_evaluator = fnFloatSexpr<OpPlus<float>>;
		}},
		{"-", [] (sexprExpr* e) {
			arithmeticCast("-",e);
			e->_evaluator = fnFloatSexpr<OpMinus<float>>;
		}},
		{"*", [] (sexprExpr* e) {
			arithmeticCast("*", e);
			e->_evaluator = fnFloatSexpr<OpMul<float>>;
		}},
		{"defun", [] (sexprExpr* e) {			}}
	};
	
	exec::exec(Sexpr in_expr)
	: _expr(in_expr)
	{
		lambda::walkSexpr(_expr,
			[] (Sexpr &expr, int depth)
			{
				sexprInteger *si = dynamic_cast<sexprInteger*>(expr.get());
				if (si)
				{
					si->type = TypeInteger;
				}
				
				sexprFloat *sf = dynamic_cast<sexprFloat*>(expr.get());
				if (sf)
				{
					sf->type = TypeFloat;
				}
			},
			[] (Sexpr &parent, int depth)
			{
				Sexpr p = parent->child();
				sexprExpr *schild = dynamic_cast<sexprExpr*>(parent.get());
				
				sexprSymbol *ss= dynamic_cast<sexprSymbol*>(p.get());
				if (ss)
				{
					std::function<void (sexprExpr*)> f = NULL;
					if (builtin.find(*ss->symbol) != builtin.end())
						f = builtin[*ss->symbol];
					
					if (f)
					{
						schild->_fnName = ss->symbol;
						schild->_child = ss->next;
						
						f(schild);
					}
				}
				
				// Scan all children for definitions...
				Sexpr child = parent->child();
				Sexpr prevChild;
				while (child)
				{
					sexprExpr *ssChild = dynamic_cast<sexprExpr*>(p.get());
					if (ssChild)
					{
						if (*(ssChild->_fnName) == std::string("defun"))
						{
							if (prevChild)
								prevChild->next = child->next;
							else
								schild->_child = child->next;
						}
					}
					
					prevChild = child;
					child = child->next;
				}
			});
	}
	
	Sexpr exec::eval()
	{
		return _expr->eval();
	}
}
