#ifndef _X86_VISITOR_
#define _X86_VISITOR_


#include "ASTVisitor.hpp"
#include "LabelMaker.hpp"
#include <deque>
#include <vector>
#include <queue>
#include <sstream>
class x86Visitor : public ASTVisitor {
private:
    void printRegDeq(std::deque<string>);
    string getReg(int);
	void comparePredicate(string, string, string);
	void generateBinOpInstr(int, string, string);
	void pushRegs();
	void popRegs();

    void createSubroutine(string);
    void deallocVar();
    void ret();
   	string getNextReg();
	string getNextStore();
	void unfoldedFunctionVisitor(NFunctionDeclaration*);
	void restoreStore(string);
	void restoreStore();
protected:
    int offset;
    stringstream text; 
    stringstream data;
	LabelMaker labelMaker;

	std::deque< std::deque<string> > callStackRegs;
    std::deque<string> freeRegs;
    std::deque<string> allRegs;
	std::deque<string> regsToRestore;

	std::queue<NFunctionDeclaration*> funcDecQueue;
public:
    x86Visitor();
    virtual void visit(NArrayAccess*);
    virtual void visit(NAssignment*);
    virtual void visit(NBinOp*);
    virtual void visit(NCharLit*);
    virtual void visit(NCodeBlock*);
    virtual void visit(NConditional*);
    virtual void visit(NDec*);
    virtual void visit(NDeclarationBlock*);
    virtual void visit(NEndIf*);
    virtual void visit(NFunctionDeclaration*);
    virtual void visit(NIdentifier*);
    virtual void visit(NInc*);
    virtual void visit(NInput*);
    virtual void visit(NInteger*);
    virtual void visit(NLoop*);
    virtual void visit(NMethodCall*);
    virtual void visit(NNullToken*);
    virtual void visit(Node*);
    virtual void visit(NParamBlock*);
    virtual void visit(NParamDeclarationBlock*);
    virtual void visit(NPrint*);
    virtual void visit(NReturn*);
    virtual void visit(NStatementList*);
    virtual void visit(NStringLit*);
    virtual void visit(NUnaryOp*);
    virtual void visit(NVariableDeclaration*);
    void init(Node*); 
	void generateFunctionDefinitions();
	 virtual string getAssembly();

};

#endif
