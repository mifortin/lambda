
#include "lambda.sexpr.util.h"

namespace lambda
{
	void walkSexpr(Sexpr &root,
				   std::function<void (Sexpr &n, int depth)> fn,
				   std::function<void (Sexpr &parent, int depth)> newDepthFn,
				   int depth)
	{
		Sexpr cur = root;
		
		do {
			fn(cur, depth);
			
			if (cur == NULL)
				return;
			
			if (cur->child() != NULL)
			{
				newDepthFn(cur, depth+1);
				Sexpr child = cur->child();
				walkSexpr(child, fn, newDepthFn, depth+1);
			}
			
			cur = cur->next;
		} while (cur != NULL);
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
						  printf("| %s", n->stringValue()->c_str());
					  else
						  printf("> Error");
					  
					  printf("\n");
				  },
				  [] (Sexpr &c, int depth)
				  {});
	}
}

