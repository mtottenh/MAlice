#ifndef _ARMVISITOR_HPP_
#define _ARMVISITOR_HPP_

#include <sstream>
#include <deque>
#include <utility>

#include "ASTVisitor.hpp"
#include "LabelMaker.hpp"

class ARMVisitor : public ASTVisitor {
private:
	void createGlobalVar(Node*, string);
protected:
	stringstream text;
	stringstream data;
	LabelMaker labelMaker;
	deque<string> freeRegs;
	deque<string> allRegs;
public:
	ARMVisitor();
	void init(Node*); 
    virtual void visit(NArrayAccess*) = 0;
    virtual void visit(NAssignment*) = 0;
    virtual void visit(NBinOp*) = 0;
    virtual void visit(NCharLit*) = 0;
    virtual void visit(NCodeBlock*) = 0;
    virtual void visit(NConditional*) = 0;
    virtual void visit(NDec*) = 0;
    virtual void visit(NDeclarationBlock*) = 0;
    virtual void visit(NEndIf*) = 0;
    virtual void visit(NFunctionDeclaration*) = 0;
    virtual void visit(NIdentifier*) = 0;
    virtual void visit(NInc*) = 0;
    virtual void visit(NInput*) = 0;
    virtual void visit(NInteger*) = 0;
    virtual void visit(NLoop*) = 0;
    virtual void visit(NMethodCall*) = 0;
    virtual void visit(NNullToken*) = 0;
    virtual void visit(Node*) = 0;
    virtual void visit(NParamBlock*) = 0;
    virtual void visit(NParamDeclarationBlock*) = 0;
    virtual void visit(NPrint*) = 0;
    virtual void visit(NReturn*) = 0;
    virtual void visit(NStatementList*) = 0;
    virtual void visit(NStringLit*) = 0;
    virtual void visit(NUnaryOp*) = 0;
    virtual void visit(NVariableDeclaration*) = 0;
};

#endif
