#include "TreeOptimiser.hpp"

TreeOptimiser::TreeOptimiser() {

}

void TreeOptimiser::visit(NArrayAccess*){}
void TreeOptimiser::visit(NAssignment*){}
void TreeOptimiser::visit(NBinOp*){}
void TreeOptimiser::visit(NCharLit*){}
void TreeOptimiser::visit(NCodeBlock*){}
void TreeOptimiser::visit(NConditional*){}
void TreeOptimiser::visit(NDec*){}
void TreeOptimiser::visit(NDeclarationBlock*){}
void TreeOptimiser::visit(NEndIf*){}
void TreeOptimiser::visit(NFunctionDeclaration*){}
void TreeOptimiser::visit(NIdentifier*){}
void TreeOptimiser::visit(NInc*){}
void TreeOptimiser::visit(NInput*){}
void TreeOptimiser::visit(NInteger*){}
void TreeOptimiser::visit(NLoop*){}
void TreeOptimiser::visit(NMethodCall*){}
void TreeOptimiser::visit(NNullToken*){}
void TreeOptimiser::visit(Node*){}
void TreeOptimiser::visit(NParamBlock*){}
void TreeOptimiser::visit(NParamDeclarationBlock*){}
void TreeOptimiser::visit(NPrint*){}
void TreeOptimiser::visit(NReturn*){}
void TreeOptimiser::visit(NStatementList*){}
void TreeOptimiser::visit(NStringLit*){}
void TreeOptimiser::visit(NUnaryOp*){}
void TreeOptimiser::visit(NVariableDeclaration*){}
void TreeOptimiser::init(Node*){} 

