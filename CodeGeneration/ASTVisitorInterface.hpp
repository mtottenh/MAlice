#ifndef _BASE_VISITOR_H_
#define _BASE_VISITOR_H_
#include "../Node/NArrayAccess.hpp" 
#include "../Node/NAssignment.hpp" 
#include "../Node/NBinOp.hpp" 
#include "../Node/NCharLit.hpp" 
#include "../Node/NCodeBlock.hpp" 
#include "../Node/NConditional.hpp" 
#include "../Node/NDec.hpp" 
#include "../Node/NDeclarationBlock.hpp" 
#include "../Node/NEndIf.hpp" 
#include "../Node/NFunctionDeclaration.hpp" 
#include "../Node/NIdentifier.hpp" 
#include "../Node/NInc.hpp" 
#include "../Node/NInput.hpp" 
#include "../Node/NInteger.hpp" 
#include "../Node/NLoop.hpp" 
#include "../Node/NMethodCall.hpp" 
#include "../Node/NNullToken.hpp" 
#include "../Node/Node.hpp" 
#include "../Node/NParamBlock.hpp" 
#include "../Node/NParamDeclarationBlock.hpp" 
#include "../Node/NPrint.hpp" 
#include "../Node/NReturn.hpp" 
#include "../Node/NStatementList.hpp" 
#include "../Node/NStringLit.hpp" 
#include "../Node/NUnaryOp.hpp" 
#include "../Node/NVariableDeclaration.hpp"
 
class ASTVisitorInterface {
public:    
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

