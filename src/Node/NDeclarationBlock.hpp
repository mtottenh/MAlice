/* Node representing a MAlice declaration block. */

#ifndef _NDECLARATIONBLOCK_HPP_
#define _NDECLARATIONBLOCK_HPP_

#include "Node.hpp"
class NDeclarationBlock : public Node {  
private:
	/* Check function for a root node. Behaves in the same way as check(). */
	int checkRoot();
public:
	/* Empty constructor, taking no child nodes. */
	NDeclarationBlock();
	
	/* Constructor taking a child node. */
	NDeclarationBlock(Node *child);

	/*                                                                           
	 * Checks that the node is semantically valid. Returns 1 if this is the      
 	 * case, 0 otherwise.                                                        
	 */
	virtual int check();
    void accept(ASTVisitor*);
};  

#endif
