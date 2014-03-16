#include <string>

std::string eg1(
	"(defun f (x) (* x x)) f + (+ 8 4) (+ 5) 7");

// I really want this type of syntax...
std::string eg2(
	"(let hello a b = + a b) (+ 3.5 (- 4 7))");

// Next step is recursion + pattern matching, (good test of tail recursion)
std::string eg3(
	"(defun fact (1) (1)) "
	"(defun fact (x) (* x (fact (- x 1)))) "
	"fact 4");

std::string eg4(
	"(defun f (x y) (+ (* x y) x)) f 2 4");

std::string simpleAdd("(+) 5 6");

std::string simpleAddFn("(defun plus () (+)) plus 5 6");

#include <iostream>
#include "lambda.scanner.h"
#include "lambda.sexpr.h"
#include "lambda.sexpr.util.h"
#include "lambda.exec.h"

int main(int argc, const char * argv[])
{
	lambda::Scanner s = lambda::Scanner(new lambda::scanner(eg3));
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

