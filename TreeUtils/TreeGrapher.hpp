#ifndef __TREE_GRAPH_H__
#define __TREE_GRAPH_H__
#include "../CodeGeneration/ASTVisitor.hpp"
#include "GraphUtils.hpp"

class TreeGrapher : public ASTVisitor {
private:

    Graph g;
    ofstream outf;
    void conditional_add(Node*);
public:
    TreeGrapher();
    TreeGrapher(string );
    string getAssembly() { return NULL; };
    void generateFunctionDefinitions(){} ; 
    void init(Node*, CodeGenerator*){} ; 
    void visit(NArrayAccess*) ;
    void visit(NAssignment*);
    void visit(NBinOp*);
    void visit(NCharLit*);
    void visit(NCodeBlock*);
    void visit(NConditional*);
    void visit(NDec*);
    void visit(NDeclarationBlock*);
    void visit(NEndIf*);
    void visit(NFunctionDeclaration*);
    void visit(NIdentifier*);
    void visit(NInc*);
    void visit(NInput*);
    void visit(NInteger*);
    void visit(NLoop*);
    void visit(NMethodCall*);
    void visit(NNullToken*);
    void visit(Node*);
    void visit(NParamBlock*);
    void visit(NParamDeclarationBlock*);
    void visit(NPrint*);
    void visit(NReturn*);
    void visit(NStatementList*);
    void visit(NStringLit*);
    void visit(NUnaryOp*);
    void visit(NVariableDeclaration*);
    void outputGraph();
   
};

#endif
