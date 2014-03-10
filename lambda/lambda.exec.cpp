
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
		
		// Params from stack...
		{S_PARAM, [] (Sexpr n, Sexpr s) {
			// Find the stack entry...
			int idx = n->value.i;
			Sexpr c = s;
			while (idx > 0)
			{
				c = c->next;
				idx--;
			}
			
			Sexpr r = c->duplicate();
			r->next = s;
			
			return r;
		}},
		
		// Params from stack...
		{S_SLIDE, [] (Sexpr n, Sexpr s) {
			Sexpr cur = s;
			
			int idx = n->value.i;
			while (idx > 0)
			{
				cur->next = cur->next->next;
				idx--;
			}
			
			return cur;
		}},
		
		// Push simple stack variables
		{S_INTEGER32, [] (Sexpr n, Sexpr s) {
			Sexpr r = n->duplicate();
			r->next = s;
			return r;
		}},
		
		{S_FLOAT32, [] (Sexpr n, Sexpr s) {
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
		}},
		
		{getStringPtr("*"), [] (Sexpr n, Sexpr s) {
			if (s->type == S_INTEGER32 && s->next->type == S_INTEGER32)
			{
				int i2 = s->value.i * s->next->value.i;
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
			[&] (Sexpr &parent, int depth)
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
						if (n->type == S_SYMBOL && n->value.symbol == getStringPtr("defun"))
						{
							// function name (symbols) (equivalent)
							
							Sexpr name = c->next;
							Sexpr vars = c->next->next;
							Sexpr defn = c->next->next->next;
							
							// Start setup...
							n->type = S_CLOSURE;
							n->value.symbol = name->value.symbol;
							
							// Keep track of numbs
							int id = 0;
							std::map<const char*, int> stackLoc;
							
							vars = vars->child;
							while (vars)
							{
								stackLoc[vars->value.symbol] = id;
								
								id++;
								vars = vars->next;
							}
							
							// Build the evaluator
							n->child = defn->child;
							lambda::walkSexpr(defn,
							  [] (Sexpr &expr, int depth) {
								  
							  },
							  [&] (Sexpr &parent, int depth) {
								  // Walk every child and replace it...
								  
								  // Save the parameters...
								  Sexpr c = parent->child;
								  while (c)
								  {
									  if (c->type == S_SYMBOL && stackLoc.find(c->value.symbol) != stackLoc.end())
									  {
										  c->type = S_PARAM;
										  c->eval = g_system.at(S_PARAM);
										  c->value.i = stackLoc.at(c->value.symbol);
									  }
									  c = c->next;
								  }
							  });
							
							// Slide the result back
							Sexpr last = n->child;
							while (last->next)
								last = last->next;
							Sexpr slide(new sexpr(S_SLIDE));
							slide->value.i = id;
							slide->eval = g_system.at(S_SLIDE);
							slide->next = NULL;
							last->next = slide;
							
							// Save the function
							g_fns[name->value.symbol] = n->child;
							
							printf("\n\nFound Function %s:\n", name->value.symbol);
							// Now print it out!
							lambda::debugPrint(n);
							printf("\n");
							
							c = NULL;
							n = NULL;
						}
						else if (n->type == S_SYMBOL)
						{
							if (g_builtin.find(n->value.symbol) != g_builtin.end())
							{
								n->type = S_EXEC;
								n->eval = g_builtin.at(n->value.symbol);
							}
							
							if (g_fns.find(n->value.symbol) != g_fns.end())
							{
								// Just reference the right symbol...
								n->type = S_NIL;
								n->eval = g_system.at(S_NIL);
								n->child = g_fns.at(n->value.symbol);
								
								Sexpr eval(new sexpr(S_EVAL));
								eval->next = n->next;
								n->next = eval;
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
						
						if (c)
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
			lambda::debugPrint(stack);
		}
		
		return stack;
	}
}
