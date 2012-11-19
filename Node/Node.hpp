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

/* Fix this class - could also be expression?*/
class NMethodCall : public NStatement {
};

class NBinOP : public NExpression {
public:
	NBinOP() { name = "Binary OP"; }
};

class NAssignment : public NStatement {
public:
 NExpression *rhs;
 Node *lhs;
 NAssignment() { name = "Assignment";}
};

class NCodeBlock : public NStatement {
public:
	NCodeBlock() { name = "block"; }
    /* add code to add a child */
};
class NDeclarationBlock : public Node {
public:
 vector<Node *> declarations;
};
class NVariableDeclaration : public NStatement {
public:
	NVariableDeclaration() { name = "Declaration"; }
};

/*class NExpressio nStatement*/

class NFunctionDeclaration : public NDeclarationBlock {

public:
	NFunctionDeclaration () { name = "FuncDec"; }
	NFunctionDeclaration (NCodeBlock *block) { 
		name = "FuncDec"; 
		children.push_back(block);
	}
	int print() const {
		cout << name ;
		return 1;
	}
};

class NConditional : public NStatement {
};

class NInput : public NStatement {
};

class NLoop : public NStatement {
};
class NInc  : public NStatement {};
class NDec : public NStatement {};
class NPrint : public NStatement { 
public:
	NPrint() { name = "Print"; }
};
class NString : public NExpression {};
#endif
