#include "NLoop.hpp"
#include "TypeDefs.hpp"

NLoop::NLoop(Node* predicate, Node* statementList) 
{
	name = "Loop";
	children.push_back(predicate);
	children.push_back(statementList);
	nodeType = LOOP;
}

int NLoop::check() {
	int isValid = 1;

	/* Is the predicate a Boolean? */
	int childType = children[0]->getType();
	if(childType != BOOLEAN) {
		printErrorHeader("loop predicate");
		error_type_mismatch_cond(childType);
		isValid = 0;
	}

	/* Are the children valid? (call Node superclass) */
	isValid &= Node::check();
	
	return isValid;
}

