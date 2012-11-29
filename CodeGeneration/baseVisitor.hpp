#ifndef _BASE_VISITOR_H_
#define _BASE_VISITOR_H_

#include "../Node/NodeIncludes.hpp"

class ASTVisitor {
public:    
    void visit(NArrayAcess*) = 0;
    void visit(NAssignment*) = 0;
    void visit(NBinOp*) = 0;
    void visit(NCharLit*) = 0;
    void visit(NCodeBlock*) = 0;
    void visit(NConditional*) = 0;
    void visit(NDec*) = 0;
    void visit(NDeclarationBlock*) = 0;
    void visit(NEndIf*) = 0;
    void visit(NFunctionDeclaration*) = 0;
    void visit(NIdentifier*) = 0;
    void visit(NInc*) = 0;
    void visit(NInput*) = 0;
    void visit(NInteger*) = 0;
    void visit(NLoop*) = 0;
    void visit(NMethodcall*) = 0;
    void visit(NNullToken*) = 0;
    void visit(Node*) = 0;
    void visit(NParamBlock*) = 0;
    void visit(NParamDeclarationBlock*) = 0;
    void visit(NPredicate*) = 0;
    void visit(NPrint*) = 0;
    void visit(NReturn*) = 0;
    void visit(NStatementList*) = 0;
    void visit(NStringLit*) = 0;
    void visit(NUnaryOp*) = 0;
    void visit(NVariableDeclaration*) = 0;
};

#endif

