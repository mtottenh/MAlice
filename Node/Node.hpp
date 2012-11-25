#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <iostream>
#include <deque>
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
typedef deque<Node *> node_children_t;

class SymbolTable;
/* Top level Base Class */
class Node {
public: /* This should be changed to private at some point. */
	SymbolTable* table;
	int nodeType;
	int type;
	int isRootNode;
	virtual int resolveType();
public:
	node_children_t children; /* This shouldn't be publicly exposed. Fix! */
	string name; /* Same here... */
	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType() const;
	virtual string getID();
	node_children_t getChildren() const;
	virtual int check();
	virtual int addTable(SymbolTable*);
	virtual int getNodeType();
	int isRoot();
	virtual int compareTypes(int, int) const;
};

#endif
