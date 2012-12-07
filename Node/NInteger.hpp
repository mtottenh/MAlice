/* Node containing an integer value. */

#ifndef _NINTEGER_HPP_
#define _NINTEGER_HPP_

#include "Node.hpp"

class NInteger : public Node {
private:
	/* Stores the integer value of the node. */
	int value;
protected:
	/* Resolves the type of the node and returns it's int representation */
	virtual int resolveType();

	/* Returns the weight of the Node. */
	virtual int calculateWeight();
public:
	/* Empty constructor. */
    NInteger();

	/* Constructor that passes the integer value the node is to contain. */
	NInteger(int);

	/*                                                                           
 	 * Checks that the node is semantically valid. Returns 1 if this is the      
 	 * case, 0 otherwise.                                                        
	 */
	virtual int check();
    virtual void accept(ASTVisitor*);
    int getValue();
};

#endif
