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
	string name;
	int type;
	vector<Node *> children;
public:
	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType() const;
	vector<Node *> getChildren() const;
	virtual int check(SymbolTable*);
};

#endif
