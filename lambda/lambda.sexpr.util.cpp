
#include "lambda.sexpr.util.h"
#include <set>

namespace lambda
{
	static void walkSexprImpl(Sexpr &root,
					std::function<void (Sexpr &n, int depth)> fn,
					std::function<void (Sexpr &parent, int depth)> newDepthFn,
					int depth,
					std::set<Sexpr> &visited)
	{
		if (visited.find(root) != visited.end())
			return;
		
		visited.insert(root);
		
		Sexpr cur = root;
		
		do {
			fn(cur, depth);
			
			if (cur == NULL)
				return;
			
			if (cur->child != NULL)
			{
				Sexpr child = cur->child;
				walkSexprImpl(child, fn, newDepthFn, depth+1, visited);
				
				newDepthFn(cur, depth+1);
			}
			
			cur = cur->next;
		} while (cur != NULL);
	}
	
	
	void walkSexpr(Sexpr &root,
				   std::function<void (Sexpr &n, int depth)> fn,
				   std::function<void (Sexpr &parent, int depth)> newDepthFn,
				   int depth)
	{
		std::set<Sexpr> r;
		walkSexprImpl(root, fn, newDepthFn, depth, r);
	}
	
	
	void debugPrint(Sexpr &root)
	{
		walkSexpr(root,
				  [] (Sexpr &n, int depth)
				  {
					  int i;
					  for (i=0; i<depth; i++)
						  printf("   ");
					  
					  if (n != NULL)
						  printf("| %s", n->stringValue().c_str());
					  else
						  printf("> Error");
					  
					  printf("\n");
				  },
				  [] (Sexpr &c, int depth)
				  {});
	}
}

