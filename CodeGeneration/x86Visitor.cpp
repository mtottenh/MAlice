#include "x86Visitor.hpp"
#include "../Node/NodeIncludes.hpp"

x86Visitor::x86Visitor() {

}


void x86Visitor::visit(NArrayAccess *node) {

}

void x86Visitor::visit(NAssignment *node) {

}

void x86Visitor::visit(NBinOp *node) {

}

void x86Visitor::visit(NCharLit *node) {

}

void x86Visitor::visit(NCodeBlock *node) {

}

void x86Visitor::visit(NConditional *node) {

}

void x86Visitor::visit(NDec *node) {

}

void x86Visitor::visit(NDeclarationBlock *node) {
    cerr << "Declaration Block" << endl;
}

void x86Visitor::visit(NEndIf *node) {

}

void x86Visitor::visit(NFunctionDeclaration *node) {

}

void x86Visitor::visit(NIdentifier *node) {

}

void x86Visitor::visit(NInc *node) {

}

void x86Visitor::visit(NInput *node) {

}

void x86Visitor::visit(NInteger *node) {

}

void x86Visitor::visit(NLoop *node) {

}

void x86Visitor::visit(NMethodCall *node) {

}

void x86Visitor::visit(NNullToken *node) {

}

void x86Visitor::visit(Node *node) {

}

void x86Visitor::visit(NParamBlock *node) {

}

void x86Visitor::visit(NParamDeclarationBlock *node) {

}

void x86Visitor::visit(NPredicate *node) {

}

void x86Visitor::visit(NPrint *node) {
    /* save any reigsters we are going to use */
    program << "push rax"; 
    program << "push rcx";    
    program << "push rbx";    
    /* evaluate what we want to print */
    node->getChild(1)->accept(this);
    /* */
    program << "int 80h\n";
    cerr << "Print Node" << endl;
}

void x86Visitor::visit(NReturn *node) {
    cerr << "Return Node" << endl;
}

void x86Visitor::visit(NStatementList *node) {
    cerr << "Statement List" << endl;
}

void x86Visitor::visit(NStringLit *node) {

}

void x86Visitor::visit(NUnaryOp *node) {
    cerr << "UnaryOp" << endl;
}

void x86Visitor::visit(NVariableDeclaration *node) {
    cerr << "VarDec" << endl;
}

void x86Visitor::createSubroutine(string name) {
    program << name << ":";    
    program << "push ebp";
    program << "mov ebp, esp";
}
    /* Create label and save ebp/esp */
    /* Allocate space on stack for local variables */
    /* Callee saved registers */
    /* Body of subroutine 
     * subroutine must leave result in rax
     */
    /* Restore callee saved registers */
    /* Deallocate local variables */
    /* Restore EBP  and return */

void x86Visitor::saveCalleeReg() {
    program << "push rbx";
    program << "push rdi";
    program << "push rsi";
}    

void x86Visitor::restoreCalleeReg() {
    /** Return from subroutine**/
    program << "pop rsi";
    program << "pop rdi";
    program << "pop rbx";
}
void x86Visitor::deallocVar() {

}
void x86Visitor::ret() {
    program << "mov esp,ebp";
    program << "pop ebp";   
    program << "ret";
}

string x86Visitor::assembledProgram() {
    return this->program.str();
}
