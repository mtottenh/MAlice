/* Node representing a request for user input in MAlice. */

#ifndef _NINPUT_HPP_
#define _NINPUT_HPP_

#include "Node.hpp"

class NInput : public Node {                                               
public:
	/* Constructor that takes a node of the thing that requires input. */
	NInput(Node*);

	/*                                                                           
 	 * Checks that the node is semantically valid. Returns 1 if this is the      
 	 * case, 0 otherwise.                                                        
 	 */
	virtual int check();
    virtual void accept(ASTVisitor*);
};  

#endif
