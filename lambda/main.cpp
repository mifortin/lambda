#include <string>

std::string eg1(
	"(+ 3.5 4 (- 4 7) 8)");

std::string eg2(
	"(defun hello (a b) (+ a b)) (+ 3.5 4 (- 4 7) 8)");


#include <iostream>
#include "lambda.scanner.h"
#include "lambda.sexpr.h"
#include "lambda.sexpr.util.h"
#include "lambda.exec.h"

int main(int argc, const char * argv[])
{
	lambda::Scanner s = lambda::Scanner(new lambda::scanner(eg2));
	lambda::Lexer l = lambda::Lexer(new lambda::lexer(s));
	
	lambda::Sexpr sxp = l->lex();
	
	printf("\n\nParse Tree:\n");
	lambda::debugPrint(sxp);

	lambda::Exec lxc = lambda::Exec(new lambda::exec(sxp));
	
	printf("\n\nParse Recall:\n");
	lambda::debugPrint(sxp);
	lambda::Sexpr e = lxc->eval();
	
	printf("\n\nExecution Tree:\n");
	lambda::debugPrint(e);

	return 0;
}

