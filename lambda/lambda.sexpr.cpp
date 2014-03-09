
#include "lambda.sexpr.h"

namespace lambda
{
	static const std::map<SexprType, const char*> g_expr2str(
		{	{S_NIL, getStringPtr("nil")},
			{S_SYMBOL, getStringPtr("symbol")},
			{S_FLOAT32, getStringPtr("float32")},
			{S_INTEGER32, getStringPtr("int32")},
			{S_EXEC, getStringPtr("execution")},
			{S_CLOSURE, getStringPtr("lambda")},
			{S_EVAL, getStringPtr("eval")}});
	
	const std::string sexpr::stringValue() const
	{
		const char *name = g_expr2str.at(type);
		
		std::stringstream s;
		
		s << name << " ";
		
		if (type == S_SYMBOL || type == S_EXEC)
			s << value.symbol;
		else if (type == S_INTEGER32)
			s << value.i;
		else if (type == S_FLOAT32)
			s << value.f;
		
		return s.str();
	}
	
	lexer::lexer(Scanner &s)
	: _s(s)		{ read(); }
	
	
	Sexpr lexer::lex()
	{
		Sexpr e = expression();
		
		if (_t->getType() != TOKEN_EOF)
			printf("Expected end of file\n");
		
		// Need a child to hold functions, etc.
		return Sexpr(new sexpr(S_NIL, e));
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
					return Sexpr(new sexpr(S_NIL));
				
				return s;
			}
			
			// New expression, let's start...
			if (_t->getType() == TOKEN_LPAREN)
			{
				read();
				
				n = Sexpr(new sexpr(S_NIL, expression()));
				
				if (_t->getType() != TOKEN_RPAREN)
					printf("Expected right parenthesis...\n");
				else
					read();
			}
			
			// Symbol
			else if (_t->getType() == TOKEN_SYMBOL)
			{
				n = Sexpr(new sexpr(S_SYMBOL, _t->stringValue()->c_str()));
				read();
			}
			
			else if (_t->getType() == TOKEN_INTEGER)
			{
				n = Sexpr(new sexpr(S_INTEGER32, (int)_t->intValue()));
				read();
			}
			
			else if (_t->getType() == TOKEN_FLOAT)
			{
				n = Sexpr(new sexpr(S_FLOAT32, (float)_t->floatValue()));
				read();
			}
			
			else if (_t->getType() == TOKEN_EOF)
			{
				if (s == NULL)
					return Sexpr(new sexpr(S_NIL));
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
