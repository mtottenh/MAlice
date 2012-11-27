#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <deque>
#include <string>
#include "../Errors/SemanticErrors.hpp"
#include "../SymbolTable/SymbolTable.hpp"

#include "FileLocation.hpp"

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
protected:
	FileLocation *loc;
	boost::shared_ptr<SymbolTable> table;
	int nodeType;
	int type;
	int isRootNode;
	virtual int resolveType() ;
	node_children_t children; /* This shouldn't be publicly exposed. Fix! */
	string name; /* Same here... */
public:	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType();
	virtual string getID();
	node_children_t getChildren() const;
 	const node_children_t* getChildrenRef() const;
	virtual int check();
	virtual int addTable(boost::shared_ptr<SymbolTable>);
	void printTable();
	int hasTable();
	string getName();
	virtual int getNodeType();
	int isRoot();
	virtual int compareTypes(int, int) const;
	void setLocation(FileLocation*);
	FileLocation* getLocation();
	virtual void printErrorHeader(const string&);
	void addChild(Node*);
	Node* getChild(unsigned int);
	int getChildrenSize();
};

#endif
