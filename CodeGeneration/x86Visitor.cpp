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
    cerr << "I bet this is the problem" << endl;
}

void x86Visitor::visit(NParamBlock *node) {

}

void x86Visitor::visit(NParamDeclarationBlock *node) {

}

void x86Visitor::visit(NPredicate *node) {

}

void x86Visitor::visit(NPrint *node) {
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
