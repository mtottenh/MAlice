#include "NLoop.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"

/* Constructors. */
NLoop::NLoop(Node* predicate, Node* statementList) {
	this->name = "Loop";
	this->nodeType = LOOP;
	children.push_back(predicate);
	children.push_back(statementList);
}

/* Public methods. */

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

void NLoop::accept(ASTVisitor *v)
{
	v->visit(this);
}
