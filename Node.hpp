#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <iostream>
#include <vector>
class Node;
class NStatement;
class NExpression;
class NVariableDeclaration;
class NFunctionDeclaration;


typedef std::vector<NStatement *> StatementList;
/*May not end up using these*/
typedef std::vector<NExpression *> ExpressionList;
typedef std::vector<NVariableDeclaration *> ViarableList;


/* Top level Base Class */
class Node {
public:
	std::string name;
	std::vector<Node *> children;
public :
	Node() { name = "Node";}
	virtual ~Node() {}
	virtual int print() const {std::cout << name; return 1;}
	std::vector<Node *> getChildren() const { return children;}
};

class NExpression : public Node {
public:
	NExpression() { name = "Exp" ;}
};

class NStatement : public Node {
public:
	NStatement () { name = "Stat"; }
};


class NInteger : public NExpression {
public:
	NInteger() { name = "Int"; }
};
class NChar : public NExpression {
	public:
	 NChar() { name = "Char";}
};
class NIdentifier : public NExpression {
	public:
	NIdentifier() { name = "ID"; }
//	NIdentifier(char *a) { name = strdup(a);
};

/* Fix this class - could also be expression?*/
class NMethodCall : public NStatement {
};

class NBinOP : public NExpression {
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
 std::vector<Node *> declarations;
};

class NCodeBlock : public NStatement {
    /* add code to add a child */
};
class NVariableDeclaration : public NStatement {
public:
	NVariableDeclaration() { name = "Declaration"; }
};

/*class NExpressio nStatement*/

class NFunctionDeclaration : public NStatement {

public:
	NFunctionDeclaration () { name = "FuncDec"; }
	NFunctionDeclaration (NCodeBlock *block) { 
		name = "FuncDec"; 
		for (int i = 0; i < children.size(); i++ ){
			if( children[i] == this) {
				std::cout << " removing func block" << std::endl;
				children.erase(children.begin()+i);
			}
		}
		children.push_back(block);

	}
	int print() const {
		std::cout << "FUNC NODE:" ;
		for (int i = 0; i < children.size(); i++) {
			
		}
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
