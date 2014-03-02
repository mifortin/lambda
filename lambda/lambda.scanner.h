
#ifndef __lambda__lambda_scanner__
#define __lambda__lambda_scanner__

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace lambda
{
	enum tokenType
	{
		TOKEN_INTEGER	= 'i',
		TOKEN_FLOAT		= 'f',
		TOKEN_LPAREN	= '(',
		TOKEN_RPAREN	= ')',
		TOKEN_EOF		= 'e',
		TOKEN_ERROR		= 'E',
		TOKEN_SYMBOL	= 'S'
	};
	
	typedef std::shared_ptr<std::string> String;
	
	String getString(const char *c);
	
	class token
	{
	public:
		token(tokenType in_t);
		
		void print();
		
		virtual ~token() {}
		
		int line, column;
		
		tokenType getType() { return _t; }
		
		virtual double floatValue() { return 0; }
		virtual long long intValue() { return 0; }
		virtual String stringValue() { return String(); }
		
	protected:
		virtual void printSpecial() {}
		
	private:
		tokenType _t;
	};
	typedef std::shared_ptr<token> Token;
	
	
	class tknInteger : public token
	{
	public:
		tknInteger(long long in_i);
		
		virtual double floatValue() { return (double)_i; }
		virtual long long intValue() { return _i; }
		
	protected:
		virtual void printSpecial();
		
	private:
		long long _i;
	};
	
	
	class tknFloat : public token
	{
	public:
		tknFloat(double in_f);
		
		virtual double floatValue() { return _f; }
		virtual long long intValue() { return (long long)_f; }
	protected:
		virtual void printSpecial();
		
	private:
		double _f;
	};
	
	
	class tknSymbol : public token
	{
	public:
		tknSymbol(String &in_str);
		
	protected:
		virtual void printSpecial();
		virtual String stringValue() { return _str; }
		
	private:
		String _str;
	};
	
	
	class scanner
	{
	public:
		scanner(std::string &in_str);
		
		Token scan();
		
	private:
		
		bool isDigit(char c);
		bool isWhite(char c);
		bool isSymbol(char c);
		void skipWhite();
		
		char read();
		char peek();
		
		// Map from a string to a unique representation
		std::map<std::string, String> _strs;
		
		std::string str;
		int			idx;
		
		int _line, _col;
	};
	typedef std::shared_ptr<scanner> Scanner;
}

#endif
