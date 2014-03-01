#include <string>

std::string eg1(
	"(+ 3.5 4 (+ 4 7) 8)");


#include <iostream>
#include "lambda.scanner.h"
#include "lambda.sexpr.h"
#include "lambda.sexpr.util.h"
#include "lambda.exec.h"

int main(int argc, const char * argv[])
{
	lambda::Scanner s = lambda::Scanner(new lambda::scanner(eg1));
	lambda::Lexer l = lambda::Lexer(new lambda::lexer(s));
	
	lambda::Sexpr sxp = l->lex();
	
	printf("\n\nParse Tree:\n");
	lambda::debugPrint(sxp);
	
	printf("\n\nExecution Tree:\n");
	lambda::Exec lxc = lambda::Exec(new lambda::exec(sxp));
	lambda::Sexpr e = lxc->eval();
	lambda::debugPrint(e);

	printf("\n\nParse Recall:\n");
	lambda::debugPrint(sxp);

	return 0;
}

