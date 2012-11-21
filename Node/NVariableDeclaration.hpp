#ifndef _NVARIABLEDECLARATION_HPP_
#define _NVARIABLEDECLARATION_HPP_

#include "Node.hpp"
#include "NIdentifier.hpp"
#include <string>

class NVariableDeclaration : public Node {
private:
	int type;
public:
	/* args are identifier & type */
	NVariableDeclaration(NIdentifier*, int);
	/* args are identifier, bit expression, type - arrays */
	NVariableDeclaration(NIdentifier*, int, Node*);
	string getID() const { return name;}
	virtual int check(SymbolTable*);
}; 

#endif
