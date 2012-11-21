#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <iostream>
#include <vector>
#include <string>
#include "../Errors/SemanticErrors.hpp"
#include "../SymbolTable/SymbolTable.hpp"

using namespace std;


/*
typedef vector<NStatement *> StatementList;
May not end up using these*//*
typedef vector<NExpression *> ExpressionList;
typedef vector<NVariableDeclaration *> ViarableList;
*/
class SymbolTable;
/* Top level Base Class */
class Node {
public:
	SymbolTable* table; /* maybe should be private, fix at some point! */
	string name;
	int type;
	vector<Node *> children;
	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType() const;
	virtual string getID();
	vector<Node *> getChildren() const;
	virtual int check();
	virtual int addTable(SymbolTable*);
};

#endif
