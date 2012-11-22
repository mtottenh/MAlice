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

class Node;
typedef vector<Node *> node_children_t;

class SymbolTable;
/* Top level Base Class */
class Node {
public:
	SymbolTable* table; /* maybe should be private, fix at some point! */
	string name;
	int type;
	node_children_t children;
	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType() const;
	virtual string getID();
	node_children_t getChildren() const;
	virtual int check();
	virtual int addTable(SymbolTable*);
};

#endif
