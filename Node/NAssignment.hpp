/* Node class for a MAlice variable/method assignment. */

#ifndef _NASSIGNMENT_HPP_
#define _NASSIGNMENT_HPP_
#include "Node.hpp"

#include "NIdentifier.hpp"
#include "NCharLit.hpp"
#include "NStringLit.hpp"

class NAssignment : public Node {
private:
	/* Fields representing the nodes on the LHS and RHS of assignment. */
	Node* lval;
	Node* rval;

	/*
	 * Check that a method call is semantically valid. Returns SUCCESS if
	 * correct, FAILURE otherwise. 
	 */
	int checkMethod();
public:
	NAssignment();

	/* Special constructors for VarDeclarationAssign */
	NAssignment(string, Node*);
	NAssignment(string, char*);
	NAssignment(string, string);

	/* General cases for Identifier */
	NAssignment(Node*, Node*);

	/* Method assignment. */
	NAssignment(Node*);

	/*
	 * Check that the node is semantically correct. Returns SUCCESS if
	 * correct, FAILURE otherwise.
	 */
	virtual int check();	
};   

#endif
