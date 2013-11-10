#include "NCharLit.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"
/* Constructors. */
NCharLit::NCharLit() {
	this->name = "<Character Literal>";
	this->type = resolveType();
	this->nodeType = CHARLIT;
	this->weight = -1;
}

NCharLit::NCharLit(char *c) {
	/* Set the character field to the char literal, minus the ' symbol. */
	this->character = *++c;
	this->name = character;
	this->type = resolveType();
	this->nodeType = CHARLIT;
	this->weight = -1;
}

/* Public methods. */

int NCharLit::check() {
	/* 
	 * This is a node representing a base type, and the parser ensures it is
	 * always valid.
	 */
	return 1;
}

/* Protected methods. */
int NCharLit::calculateWeight() {
	/* A char literal is a constant; weight 1. */
	return 1;
}

void NCharLit::accept(ASTVisitor *v) {
	v->visit(this);
}

/* Private methods. */

int NCharLit::resolveType() {
	/* A character literal is always of type character! (letter in MAlice). */
	return TCHAR;
}
