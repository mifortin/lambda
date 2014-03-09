
#include "lambda.scanner.h"

namespace lambda {
	struct customSz
	{
		const char *_c;
		customSz(const char *c) : _c(c) {};
		
		bool operator<(const customSz& o) const
		{
			return strcmp(_c, o._c) < 0;
		}
		
		bool operator==(const customSz& o) const
		{
			return strcmp(_c, o._c) == 0;
		}
	};
	
	static std::map<customSz, String> allStrings;
	
	String &getString(const char *c)
	{
		if (allStrings.find(c) == allStrings.end())
		{
			String n = String(new std::string(c));
			allStrings[n->c_str()] = n;
		}
		return allStrings[c];
	}
	
	const char* getStringPtr(const char *c)
	{
		return getString(c)->c_str();
	}
	
	
	token::token(tokenType in_t)
	: _t(in_t)		{}
	
	void token::print()
	{
		printf("%4i %4i %c : ", line, column, (char)_t);
		printSpecial();
		printf("\n");
	}
	
	tknInteger::tknInteger(long long in_i)
	: token(TOKEN_INTEGER)
	, _i(in_i)		{}
	
	
	void tknInteger::printSpecial()
	{
		printf("%lld", _i);
	}
	
	
	tknFloat::tknFloat(double in_f)
	: token(TOKEN_FLOAT)
	, _f(in_f)		{}
	
	
	void tknFloat::printSpecial()
	{
		printf("%f", _f);
	}
	
	
	tknSymbol::tknSymbol(String &in_str)
	: token(TOKEN_SYMBOL)
	, _str(in_str)	{}
	
	
	void tknSymbol::printSpecial()
	{
		printf("%s", _str->c_str());
	}

	
	scanner::scanner(std::string &in_str)
	: str(in_str), idx(0), _line(1), _col(1)
	{}
	
	
	Token scanner::scan()
	{
		Token r;
		
		skipWhite();
		
		char c = peek();
		if (c == '\0')
			r = Token(new token(TOKEN_EOF));
		
		else if (isDigit(c))
		{
			long long i = c - '0';
			read();
			c = peek();
			while (isDigit(c))
			{
				i = 10 * i +  c - '0';
			
				if (isDigit(c))
					read();
				c = peek();
			}
			if (c == '.')
			{
				read();
				c = peek();
				
				double d =	1;
				while (isDigit(c))
				{
					i = 10*i + c - '0';
					d *= 10;
					if (isDigit(c))
						read();
					c = peek();
				}
				
				r = Token(new tknFloat((double)i / d));
			}
			else
				r = Token(new tknInteger(i));
		}
		else if (c == '(')
		{
			read();
			r = Token(new token(TOKEN_LPAREN));
		}
		else if (c == ')')
		{
			read();
			r = Token(new token(TOKEN_RPAREN));
		}
		else if (isSymbol(c))
		{
			std::string s;
			s += c;
			read();
			
			c = peek();
			while (isSymbol(c) || isDigit(c))
			{
				s += c;
				read();
				c = peek();
			}
			
			if (_strs.find(s) == _strs.end())
			{
				_strs[s] = String(new std::string(s));
			}
			
			r = Token(new tknSymbol(_strs[s]));
		}
		else
		{
			read();
			r = Token(new token(TOKEN_ERROR));
		}
		
		r->line = _line;
		r->column = _col;
		
		return r;
	}
	
	bool scanner::isDigit(char c)
	{
		return c >= '0' && c <= '9';
	}
	
	bool scanner::isWhite(char c)
	{
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}
	
	bool scanner::isSymbol(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '+' || c == '-' || c == '/' || c == '=';
	}
	
	void scanner::skipWhite()
	{
		while (isWhite(peek()))
			read();
	}
	
	char scanner::read()
	{
		if (str.length() <= idx)
			return '\0';
		
		char r = str[idx++];
		
		if (r == '\n')
		{
			_line++;
			_col = 0;
		}
		_col++;
		
		return r;
	}
	
	char scanner::peek()
	{
		if (str.length() <= idx)
			return '\0';
		
		return str[idx];
	}

}