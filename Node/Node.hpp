#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node;
class NStatement;
class NExpression;
class NVariableDeclaration;
class NFunctionDeclaration;


typedef vector<NStatement *> StatementList;
/*May not end up using these*/
typedef vector<NExpression *> ExpressionList;
typedef vector<NVariableDeclaration *> ViarableList;


/* Top level Base Class */
class Node {
public:
	string name;
	vector<Node *> children;
public :
	Node() { name = "Node";}
	virtual ~Node() {}
	Node(Node *a) { children.push_back(a); name = "Node"; }
	virtual int print() const {cout << name; return 1;}
	vector<Node *> getChildren() const { return children;}
};

#endif
