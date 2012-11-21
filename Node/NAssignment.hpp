#ifndef _NASSIGNMENT_HPP_
#define _NASSIGNMENT_HPP_
#include "Node.hpp"

#include "NIdentifier.hpp"
#include "NCharLit.hpp"
#include "NString.hpp"
class NAssignment : public Node {
private:
	Node* lval;
	Node* rval;
public:
	NAssignment();
	/* special cases for VarDeclarationAssign */
	NAssignment(string, Node*);
	NAssignment(string, char*);
	NAssignment(string, string);
	/* General cases for Identifier */
	NAssignment(Node*, Node*);
	NAssignment(Node*, char*);
	/* Add cases for assigning things to strings */
	virtual int check();	
};   

#endif
