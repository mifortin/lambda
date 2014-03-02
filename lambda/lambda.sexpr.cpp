
#include "lambda.sexpr.h"

namespace lambda
{	
	lexer::lexer(Scanner &s)
	: _s(s)		{ read(); }
	
	
	Sexpr lexer::lex()
	{
		Sexpr e = expression();
		
		if (_t->getType() != TOKEN_EOF)
			printf("Expected end of file\n");
		
		// Need a child to hold functions, etc.
		return Sexpr(new sexprExpr(e));
	}
	
	
	Sexpr lexer::expression()
	{
		Sexpr s;
		Sexpr end;
		
		while (1)
		{
			Token cur = _t;
			Sexpr n;
			
			// End of expression, let's bail!
			if (_t->getType() == TOKEN_RPAREN)
			{
				if (s == NULL)
					return Sexpr(new sexpr());
				
				return s;
			}
			
			// New expression, let's start...
			if (_t->getType() == TOKEN_LPAREN)
			{
				read();
				
				n = Sexpr(new sexprExpr(expression()));
				
				if (_t->getType() != TOKEN_RPAREN)
					printf("Expected right parenthesis...\n");
				else
					read();
			}
			
			// Symbol
			else if (_t->getType() == TOKEN_SYMBOL)
			{
				n = Sexpr(new sexprSymbol(_t->stringValue()));
				read();
			}
			
			else if (_t->getType() == TOKEN_INTEGER)
			{
				n = Sexpr(new sexprInteger((int)_t->intValue()));
				read();
			}
			
			else if (_t->getType() == TOKEN_FLOAT)
			{
				n = Sexpr(new sexprFloat(_t->floatValue()));
				read();
			}
			
			else if (_t->getType() == TOKEN_EOF)
			{
				if (s == NULL)
					return Sexpr(new sexpr());
				return s;
			}
			
			else
			{
				printf("Error parsing token...");
				read();
			}
			
			if (s == NULL)
				s = n;
			else
				end->next = n;
			
			end = n;
		}
	}
	
	
	void lexer::read()
	{
		_t = _s->scan();
		_t->print();
	}
}
