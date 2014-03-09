
#include "lambda.exec.h"

namespace lambda
{
	// First is node, second is stack.  Return new stack.
	static std::map<SexprType, std::function<Sexpr (Sexpr, Sexpr)>> g_system
	({
		// Closures will be pushed then applied later...
		{S_NIL, [] (Sexpr n, Sexpr s) {
			Sexpr r(new sexpr(S_CLOSURE));
			r->next = s;
			r->child = n->child;
			return r;
		}},
		
		// Push simple stack variables
		{S_INTEGER32, [] (Sexpr n, Sexpr s) {
			Sexpr r = n->duplicate();
			r->next = s;
			return r;
		}}
	});
	
	static std::map<const char*, std::function<Sexpr (Sexpr, Sexpr)>> g_builtin
	({
		// Built-in mathematical ops
		{getStringPtr("+"), [] (Sexpr n, Sexpr s) {
			if (s->type == S_INTEGER32 && s->next->type == S_INTEGER32)
			{
				int i2 = s->value.i + s->next->value.i;
				Sexpr r = Sexpr(new sexpr(S_INTEGER32, i2));
				r->next = s->next->next;
				return r;
			}
			
			return s;
		}}
	});
	
	exec::exec(Sexpr in_expr)
	: _expr(in_expr)
	{
		lambda::walkSexpr(_expr,
			[] (Sexpr &expr, int depth)
			{
			},
			[] (Sexpr &parent, int depth)
			{
				// Need to recreate the nodes...
				Sexpr c = parent->child;
				if (c)
				{
					Sexpr n;
					
					while (c)
					{
						Sexpr cpy = c->duplicate();
						cpy->next = n;
						n = cpy;
						
						// Resolve this symbol (it execs)
						if (n->type == S_SYMBOL)
						{
							if (g_builtin.find(n->value.symbol) != g_builtin.end())
							{
								n->type = S_EXEC;
								n->eval = g_builtin.at(n->value.symbol);
							}
						}
						else
						{
							if (g_system.find(n->type) != g_system.end())
							{
								n->eval = g_system.at(n->type);
							}
							
							if (n->type == S_NIL)
							{
								Sexpr eval(new sexpr(S_EVAL));
								eval->next = n->next;
								n->next = eval;
							}
						}
						
						c = c->next;
					}
					
					parent->child = n;
				}
				
			});
	}
	
	Sexpr exec::eval()
	{
		// For each child in the VM...
		Sexpr stack;
		
		Sexpr c = _expr->child;
		
		// Stack of stacks...
		std::vector<Sexpr> dump;
		
		while (c)
		{
			// Function pointer for evaluation?
			printf("X %s\n", c->stringValue().c_str());
			if (c->eval)
			{
				stack = c->eval(c, stack);
			}
			
			if (c->type == S_EVAL)
			{
				dump.push_back(c);
				c = stack->child;
				stack = stack->next;
			}
			else
			{
				c = c->next;
				
				while (c == NULL && dump.size() > 0)
				{
					c = dump.back();
					c = c->next;
					dump.pop_back();
				}
			}
		}
		
		return stack;
	}
}
