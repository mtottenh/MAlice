#include "x86Visitor.hpp"
#include "../Node/NodeIncludes.hpp"

x86Visitor::x86Visitor() {

}


void x86Visitor::visit(NArrayAccess*) {

}

void x86Visitor::visit(NAssignment*) {

}

void x86Visitor::visit(NBinOp*) {

}

void x86Visitor::visit(NCharLit*) {

}

void x86Visitor::visit(NCodeBlock*) {

}

void x86Visitor::visit(NConditional*) {

}

void x86Visitor::visit(NDec*) {

}

void x86Visitor::visit(NDeclarationBlock*) {
    cerr << "Declaration Block" << endl;
}

void x86Visitor::visit(NEndIf*) {

}

void x86Visitor::visit(NFunctionDeclaration*) {

}

void x86Visitor::visit(NIdentifier*) {

}

void x86Visitor::visit(NInc*) {

}

void x86Visitor::visit(NInput*) {

}

void x86Visitor::visit(NInteger*) {

}

void x86Visitor::visit(NLoop*) {

}

void x86Visitor::visit(NMethodCall*) {

}

void x86Visitor::visit(NNullToken*) {

}

void x86Visitor::visit(Node*) {

}

void x86Visitor::visit(NParamBlock*) {

}

void x86Visitor::visit(NParamDeclarationBlock*) {

}

void x86Visitor::visit(NPredicate*) {

}

void x86Visitor::visit(NPrint*) {

}

void x86Visitor::visit(NReturn*) {

}

void x86Visitor::visit(NStatementList*) {

}

void x86Visitor::visit(NStringLit*) {

}

void x86Visitor::visit(NUnaryOp*) {

}

void x86Visitor::visit(NVariableDeclaration*) {
    cerr << "VarDec" << endl;
}
