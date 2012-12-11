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

	/* Reserve space for global vars */
	for(it = t->start(); it != t->end(); ++it) {
		label = it->first + labelSuffix;
		data << ".comm " << label << " " << it->second->getSize() << endl;
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

/* Pushes registers that are in use. */
void ARMVisitor::pushRegs() {
	deque<string>::iterator it;
	for(it = allRegs.begin(); it != allRegs.end(); ++it) {
		if(find(freeRegs.begin(), freeRegs.end(), (*it)) == freeRegs.end()) {
			text << "\tpush " << (*it) << endl;
		}
	}
	callStackRegs.push_front(freeRegs);
	freeRegs = allRegs;
}

/* Pops registers that are (were) in use. */
void ARMVisitor::popRegs() {
	freeRegs = callStackRegs.front();
	callStackRegs.pop_front();

	deque<string>::iterator it;
	for(it = allRegs.begin(); it != allRegs.end(); ++it) {
		if(find(freeRegs.begin(), freeRegs.end(), (*it)) == freeRegs.end()) {
			text << "\tpop " << (*it) << endl;
		}
	}
}

string ARMVisitor::getNextReg() {
	string res;
	if(freeRegs.size() == 1) {
		res = freeRegs.front();
	} else {
		res = freeRegs.front();
		freeRegs.pop_front();
	}

	return res;
}

string ARMVisitor::getNextStore() {
	string res;
	if (freeRegs.size() == 1) {
		res = "[sp]"; /* r13 = sp. this might need to be r13. */
	} else {
		res = getNextReg();
	}
	return res;
}

void ARMVisitor::restoreStore(string store) {
	if(find(allRegs.begin(), allRegs.end(), store) != allRegs.end()) {
		freeRegs.push_front(store);
	}
}

void ARMVisitor::restoreStore() {
	for(unsigned int i = 0; i < regsToRestore.size(); ++i) {
		restoreStore(regsToRestore[i]);
	}
	regsToRestore.clear();
}
