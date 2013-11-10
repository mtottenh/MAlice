#include "../CodeGeneration/ASTVisitor.hpp"
#include "NNullToken.hpp"

NNullToken::NNullToken() {
    name = "Null Token";
}

void NNullToken::accept(ASTVisitor *v) {
    v->visit(this);
}
