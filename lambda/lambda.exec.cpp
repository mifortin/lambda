
#include "lambda.exec.h"

namespace lambda
{
	static std::map<std::string, fnSexpr> builtin
	= {
		{"+", [] (Sexpr e) {
			float f = 0;
			
			while (e != NULL)
			{
				f += e->floatValue();
				e = e->next;
			}
			
			return Sexpr(new sexprFloat(f));
		}},
		{"-", [] (Sexpr e) {
			float f = 0;
			
			if (e)
			{
				f = e->floatValue();
				e = e->next;
			}
			
			while (e != NULL)
			{
				f -= e->floatValue();
				e = e->next;
			}
			
			return Sexpr(new sexprFloat(f));
		}},
		{"\\", [] (Sexpr e) {
			
			return Sexpr();
		}}
	};
	
	exec::exec(Sexpr in_expr)
	: _expr(in_expr)
	{
		lambda::walkSexpr(in_expr,
			[] (Sexpr &expr, int depth)
			{
				
			},
			[] (Sexpr &parent, int depth)
			{
				Sexpr p = parent->child();
				sexprExpr *schild = dynamic_cast<sexprExpr*>(parent.get());
				
				sexprSymbol *ss= dynamic_cast<sexprSymbol*>(p.get());
				if (ss)
				{
					fnSexpr f = NULL;
					if (builtin.find(*ss->symbol) != builtin.end())
						f = builtin[*ss->symbol];
					
					if (f)
					{
						sexprEval *se = new sexprEval(ss->symbol, f);
						se->next = ss->next;
						
						schild->_child = Sexpr(se);
					}
				}
			});
	}
	
	Sexpr exec::eval()
	{
		return _expr->eval();
	}
}
