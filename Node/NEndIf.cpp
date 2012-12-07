#include "../CodeGeneration/ASTVisitor.hpp"
#include "NEndIf.hpp"
#include "TypeDefs.hpp"

NEndIf::NEndIf() {
	this->name = "EndIf";
	this->nodeType = ENDIF;
}


void NEndIf::accept(ASTVisitor *v) {
    v->visit(this);
}
