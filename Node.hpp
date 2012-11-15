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
private:
	std::string name;
	std::vector<Node *> children;
public :
	Node() { name = "Node";}
	virtual ~Node() {}
	virtual int print() const {std::cout << name; return 1;}
	std::vector<Node *> getChildren() const { return children;}
};

class NExpression : public Node {
};

class NStatement : public Node {
};


class NInteger : public NExpression {
};
class NChar 	: public NExpression {
};
class NIdentifier : public NExpression {
};

/* Fix this class - could also be expression?*/
class NMethodCall : public NStatement {
};

class NBinOP : public NExpression {
};

class NAssignment : public NStatement {
 NExpression *rhs;
 Node *lhs;
};

class NCodeBlock : public NStatement {
    /* add code to add a child */
};

class NVariableDeclaration : public NStatement {
 NCodeBlock *block;
};

/*class NExpressio nStatement*/

class NFunctionDeclaration : public NStatement {

};

class NConditional : public NStatement {
};

class NInput : public NStatement {
};

class NLoop : public NStatement {
};
class NInc  : public NStatement {};
class NDec : public NStatement {};
class NPrint : public NStatement {};
class NString : public NExpression {};
#endif
