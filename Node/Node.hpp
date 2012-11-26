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
typedef struct file
{
    int startLine;
    int endLine;
    int startColumn;
    int endColumn;
} FileLocation;



class SymbolTable;
/* Top level Base Class */
class Node {
private:
	FileLocation loc;
public: /* This should be changed to private at some point. */
	SymbolTable* table;
	int nodeType;
	int type;
	int isRootNode;
	virtual int resolveType() ;
public:
	node_children_t children; /* This shouldn't be publicly exposed. Fix! */
	string name; /* Same here... */
	Node();
	Node(Node*);
	virtual ~Node();
	virtual int print() const;
	virtual int getType() ;
	virtual string getID();
	node_children_t getChildren() const;
	virtual int check();
	virtual int addTable(SymbolTable*);
	virtual int getNodeType();
	int isRoot();
	virtual int compareTypes(int, int) const;
	void setLocation(FileLocation);
	FileLocation getLocation();
	virtual void printErrorHeader();
	void printErrorHeader(const string&);
};

#endif
