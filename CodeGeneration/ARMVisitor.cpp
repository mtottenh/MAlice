#include "ARMVisitor.hpp"

ARMVisitor::ARMVisitor() {
	freeRegs.clear();
	allRegs.clear();

	/* Push general purpose regs to allRegs. */
	/* R0..R12 are general purpose regs. Reserve R0 for method returns. */
	allRegs.push_back("r1");
	allRegs.push_back("r2");
	allRegs.push_back("r3");
	allRegs.push_back("r4");
	allRegs.push_back("r5");
	allRegs.push_back("r6");
	allRegs.push_back("r7");
	allRegs.push_back("r8");
	allRegs.push_back("r9");
	allRegs.push_back("r10");
	allRegs.push_back("r11");
	allRegs.push_back("r12");

	/* Set freeRegs to allRegs. (all regs are free at the start!) */
	freeRegs = allRegs;
}

void ARMVisitor::visit(NArrayAccess* node) {

}

void ARMVisitor::visit(NAssignment* node) {

}

void ARMVisitor::visit(NBinOp* node) {

}

void ARMVisitor::visit(NCharLit* node) {

}

void ARMVisitor::visit(NCodeBlock* node) {

}

void ARMVisitor::visit(NConditional* node) {

}

void ARMVisitor::visit(NDec* node) {

}

void ARMVisitor::visit(NDeclarationBlock* node) {

}

void ARMVisitor::visit(NEndIf* node) {

}

void ARMVisitor::visit(NFunctionDeclaration* node) {

}

void ARMVisitor::visit(NIdentifier* node) {

}

void ARMVisitor::visit(NInc* node) {

}

void ARMVisitor::visit(NInput* node) {

}

void ARMVisitor::visit(NInteger* node) {

}

void ARMVisitor::visit(NLoop* node) {

}

void ARMVisitor::visit(NMethodCall* node) {

}

void ARMVisitor::visit(NNullToken* node) {

}

void ARMVisitor::visit(Node* node) {

}

void ARMVisitor::visit(NParamBlock* node) {

}

void ARMVisitor::visit(NParamDeclarationBlock* node) {

}

void ARMVisitor::visit(NPrint* node) {

}

void ARMVisitor::visit(NReturn* node) {

}

void ARMVisitor::visit(NStatementList* node) {

}

void ARMVisitor::visit(NStringLit* node) {

}

void ARMVisitor::visit(NUnaryOp* node) {

}

void ARMVisitor::visit(NVariableDeclaration* node) {

}

void ARMVisitor::init(Node* root) {
	/* Set up data section. */
	data << ".data" << endl;

	/* Get all global variables and string literals. */
	boost::shared_ptr<SymbolTable> t = root->getTable();
	table_t::iterator it;

	string labelSuffix = labelMaker.getNewLabel();
	string label;

	for(it = t->start(); it != t->end(); ++it) {
		label = it->first + labelSuffix;
		it->second->setLabel(label);
	}

	/* Set up text and main. */
	text << ".text" << endl;
	text << ".global main" << endl;
	text << "main:" << endl;
	text << "\tbl _hattalabel1" << endl;
	text << "\tldr r0, =" << EXIT_SUCCESS << endl;
	text << "\tb exit" << endl;
}

void ARMVisitor::createGlobalVar(Node* node, string label) {
	int type = node->getType();
	
	data << label << ":" << endl;

	switch(type) {
	case TNUMBER:
		data << "\t.int " << node->getValue() << endl;
	case TCHAR:
	case TSTRING:
		data << "\t.asciz " << node->getID() << endl;
		break;
	default:
		cerr << "Tried to declare global var of invalid type!" << endl;
		break;
	}
}
