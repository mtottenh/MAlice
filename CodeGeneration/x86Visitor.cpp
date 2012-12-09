#include "x86Visitor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "../Node/NodeIncludes.hpp"
/* Register conventions -
 * RAX stores the result of any procedure or function
 * a function can have an arbitrary number of paramters which are stored on the
 * stack in reverse order
 */



x86Visitor::x86Visitor() : labelMaker() {
    offset = 0;
    freeRegs.clear();
    /* general purpose registers */
	/* TODO - rax..rdx aren't necessarily general purpose - see IDIV! */
    allRegs.clear();
    allRegs.push_back("r8");
    allRegs.push_back("r9");
    allRegs.push_back("r10");
    allRegs.push_back("r11");
    allRegs.push_back("r12");
    allRegs.push_back("r13");
    allRegs.push_back("r14");
    allRegs.push_back("r15");
    allRegs.push_back("rax");
    allRegs.push_back("rbx");
    allRegs.push_back("rcx");
    allRegs.push_back("rdx");
    allRegs.push_back("rsi");
    allRegs.push_back("rdi");
    freeRegs = allRegs;
}



void x86Visitor::visit(NArrayAccess *node) {

}

void x86Visitor::visit(NAssignment *node) {
	cerr << "Node: Assignment" << endl;
    int rval = node->getChildrenSize() - 1;
    int lval = rval-1;

    node->getChild(rval)->accept(this);
    if (node->getChildrenSize() > 1) {
       Node* leftChildID = node->getChild(lval);
       Node* declaredNode = node->getTable()->lookup(leftChildID->getID());
       text << "\tmov " << "[" << declaredNode->getLabel() << "]";
       text << ", " << freeRegs.front() <<  endl;
        //Assign the result to the variable
    }
}
/* TODO: still need to implement a register saving mechanism 
 * maybe paramerterise visit with a list of reg's it can use
 * like the tutorial.
 */
void x86Visitor::generateBinOpInstr(int op, string resultReg, string nxtReg) {
	
    switch(op) {
        case OR:
		case LOR:
            text << "\tor " << resultReg << "," << nxtReg << endl;
			break;
		case AND:
        case LAND:
            text << "\tand " << resultReg << "," << nxtReg << endl;
			break;
        case LEQU:
			comparePredicate("sete", resultReg, nxtReg);	
			break; 
        case LLTHAN:
			comparePredicate("setl", resultReg, nxtReg);
			break;
        case LLTHANEQ:
            comparePredicate("setle", resultReg, nxtReg);
			break;
        case LGTHAN:
            comparePredicate("setg", resultReg, nxtReg);
			break;
        case LGTHANEQ:
            comparePredicate("setge", resultReg, nxtReg);
			break;
        case LNOTEQU:
            comparePredicate("setne", resultReg, nxtReg);
			break;
        case XOR:
            text << "\txor " << resultReg << ", " << nxtReg << endl;
			break;
        case PLUS:
            text << "\tadd " << resultReg << ", " << nxtReg << endl;
			break;
        case DASH:
            text << "\tsub " << resultReg << ", " << nxtReg << endl;
			break;
        case MULT:
            text << "\timul " << resultReg << ", " << nxtReg << endl;
			break;
        case DIV:
			text << "\tpush rax" << endl;
			text << "\tpush rdx" << endl;
			/* not sure if rax or rdx... see intel documentation */
			text << "\tmov " << resultReg << ", rdx" << endl;
            text << "\tcqo" << endl;
			text << "\tidiv " << nxtReg << endl;
			/* quotient lives in rax, move to resultReg */
			text << "\tmov " << resultReg << ", rax" << endl;
			text << "\tpop rdx" << endl;
			text << "\tpop rax" << endl;
			break;
        case MOD:
            text << "\tpush rax" << endl;
			text << "\tpush rdx" << endl;
			/* not sure if rax or rdx... see intel documentation */
			text << "\tmov " << resultReg << ", rdx" << endl;
            text << "\tcqo" << endl;
			text << "\tidiv " << nxtReg << endl;
			/* remainder lives in rdx, move to resultReg */
			text << "\tmov " << resultReg << ", rdx" << endl;
			text << "\tpop rdx" << endl;
			text << "\tpop rax" << endl;
 			break;
    }


}
void x86Visitor::visit(NBinOp *node) {
	string resultReg, nxtReg;
    if (node->getChild(0)->getWeight() > node->getChild(1)->getWeight()) {
        cerr << "\nBinOp: evaluating left first\n";
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
	   	generateBinOpInstr(node->getOp(),resultReg,nxtReg);
		restoreStore(nxtReg);
		restoreStore(resultReg);
    } else {
		/* TODO PopnSwap - BUBBLE POP */
        cerr << "\nBinOp: evaluating right first\n";
        node->getChild(1)->accept(this);
		nxtReg = getNextReg();
        node->getChild(0)->accept(this);
		resultReg = getNextStore();
		cerr << "Result Register: " << resultReg << "\t Next Register: " << nxtReg << endl;
	   	generateBinOpInstr(node->getOp(), resultReg, nxtReg);
		restoreStore(nxtReg);
		restoreStore(resultReg);

    }
 

}

void x86Visitor::comparePredicate(string setInstr, string resultReg,
									string nxtReg) {
		/* 
		 * SETX instructions check EFLAGS for equality, less than etc.. and
		 * set a BYTE if the condition matches X. We use MOVZX to put it in
		 * the 64 bit register.
		 */ 
		text << "\tcmp " << resultReg << ", " << nxtReg << endl;
		text << "\tpush rax" << endl;
		text << "\t" << setInstr << " al" << endl;
		text << "\tmovzx " << resultReg << ", al" << endl;
		text << "\tpop rax" << endl;
}

/* TODO: As we have an output charlit macro I'm sure that we
 * no longer need to store this in the data segment
 * i.e the only reason we wanted to store it in .data was
 * because we didnt have a way of printing it before
 */
void x86Visitor::visit(NCharLit *node) {
	cerr << "Node: Char Lit " << endl;
    text << "\t mov " << freeRegs.front() << ", '" << node->getID() << "'\n"; 
}

void x86Visitor::visit(NCodeBlock *node) {
    /* Visit the statement list, and declaration list if it exists. */
	cerr << "Node: CodeBlock" << endl;
    for(int i = 0; i < node->getChildrenSize(); ++i) {
        node->getChild(i)->accept(this);
    }
}

void x86Visitor::visit(NConditional *node) {
	cerr << "Node: Coditional " << endl;
    bool startConditional = this->labelMaker.needsNewEndCondLabel();
    string endLabel = this->labelMaker.getEndCondLabel();
    string elseLabel = this->labelMaker.getNewLabel();

    // Visit the predicate to print out condition
    node->getChild(0)->accept(this);
	string resultReg = this->getNextReg();

    // If condition is false, jump to next condition

	text << "\tcmp " << resultReg << ", 1" << endl;
    text << "\tjne " << elseLabel << endl;
	restoreStore(resultReg);

    // Otherwise, execute the statement list, first pushing
    // the end condition label onto the stack
    this->labelMaker.pushEndCondLabel();
    node->getChild(1)->accept(this);

    this->labelMaker.popEndCondLabel();

    // Return to end of conditional
    text << "\tjmp " << endLabel << endl;

    // Print out code for rest of cases
    text << elseLabel << ":" << endl;
    node->getChild(2)->accept(this);


    if (startConditional)  {
        text << endLabel << ":" << endl;
        this->labelMaker.resetEndCondLabel();
    }
}

void x86Visitor::visit(NEndIf *node) {
}

/* NDeclarationBlock is our 'entry' point - its not exclusve though, ie every 
 * function has one
 */
void x86Visitor::visit(NDeclarationBlock *node) {

    /* if we have a delcaration block just visit every child*/
    offset = 0;
    if (!node->isRoot()) {
        for (int i = 0; i < node->getChildrenSize(); i++) {
            node->getChild(i)->accept(this);
        }
    } else {
        for (int i = 0; i < node->getChildrenSize(); i++) {
            if (node->getChild(i)->getNodeType() == VARDEC) {
                cerr << "I CANNE HOLD EM CAP'N " << endl;
            } else {
                node->getChild(i)->accept(this);
            }
        }

    }
    
}

void x86Visitor::visit(NFunctionDeclaration *node) {
    funcDecQueue.push(node);
}

void x86Visitor::visit(NIdentifier *node) {
	cerr << "Node: Identifier" << endl;
    Node *declarationNode = node->getTable()->lookup(node->getID());
    text << "\tmov " << freeRegs.front() << ", " << "["
         << declarationNode->getLabel() << "]" << endl; 

}

void x86Visitor::visit(NInc *node) {
    /* Visit the node containing the expression to be incremented. */
    node->getChild(0)->accept(this);
	boost::shared_ptr<SymbolTable> table = node->getTable();
	string childID = node->getChild(0)->getID();
	Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
//	string store = getNextStore();
	string store = freeRegs.front();
	text << "\tinc " << store << "\n";
	text << "\tmov " << "[" << childDeclarationNode->getLabel() << "], "
		 << store << endl;
//	restoreStore(
}

void x86Visitor::visit(NDec *node) {
    /* Visit the node containing the expression to be decremented. */
    node->getChild(0)->accept(this);
	boost::shared_ptr<SymbolTable> table = node->getTable();
	string childID = node->getChild(0)->getID();
	Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
//	string store = getNextStore();
	string store = freeRegs.front();
	text << "\tdec " << store << "\n";
	text << "\tmov " << "[" << childDeclarationNode->getLabel() << "], "
		 << store << endl;
}

void x86Visitor::visit(NInput *node) {

}

void x86Visitor::visit(NInteger *node) {
    text << "\n\tmov " << freeRegs.front() << ", " << node->getValue();
    text << endl;

}

void x86Visitor::visit(NLoop *node) {
    /* Get a label for the start and end of the while loop. Print start label.*/
    string startLabel = labelMaker.getNewLabel();
    string endLabel = labelMaker.getNewLabel();
    text << startLabel << ":" << endl;

    /* 
     * Visit condition. Assuming that 0 is stored in flags register if false,
     * jump to endLabel if the condition does not hold.
     */
    node->getChild(0)->accept(this);
    text << "\tjz " << endLabel << "\n";
    
    /* Visit the statement list of the loop node, then recheck condition. */
    node->getChild(1)->accept(this);
    text << "\tjmp " << startLabel << "\n";

    /* Print end label finish. */
    text << endLabel << ":\n";

}

void x86Visitor::visit(NMethodCall *node) {
    if (node->getChildrenSize() > 0) {
        node->getChild(0)->accept(this);
    }
    cout << "Node: Function or Procedure Call" << endl;
    text << "\tcall _" << node->getTable()->
                lookup(node->getID())->getLabel() << endl;
}

void x86Visitor::visit(NNullToken *node) {
    cout << "Node: NNullToken" << endl;
}

void x86Visitor::visit(Node *node) {

}

void x86Visitor::visit(NParamBlock *node) {
    /* Here we will need to push the children onto the stack
     * in the reverse order 
     */
    unsigned int numChildren =  node->getChildrenSize();

      for (unsigned int i = 0; i < numChildren; i++) {
          
          node->getChild(i)->accept(this);
          text << "\t; push child node" << endl;
          text << "\tpush " << freeRegs.front() << endl;
      }
}

void x86Visitor::visit(NParamDeclarationBlock *node) {
    /* This is a list of variable declarations, we need to 
     * set the labels of them to [rsp+offset] where offset starts at
     * 16 and goes up in 8's
     */
     unsigned int numChildren = node->getChildrenSize();
    unsigned int offset = 16;
    for(unsigned int i = 0; i < numChildren; i++) {
        string label = "rsp+";
        label = label + boost::lexical_cast<string>(offset);
        node->getChild(i)->setLabel(label);
        offset+=8;
    }
    
}
/*TODO: change this from output char to output string or something..*/
void x86Visitor::visit(NPrint *node) {
	cerr << "Node: Print";
    int type,nodeType;
    nodeType = node->getChild(0)->getNodeType();
    type = node->getChild(0)->getType();
    if(nodeType == CHARLIT) {
    /* We can use the macro defed in system.inc */
        text << "output.char ";
 	    node->getChild(0)->accept(this);   
    }
    if(nodeType == STRINGLIT) {
 	    node->getChild(0)->accept(this);   
        text << "\tpush rax\nmov rax," <<node->getChild(0)->getLabel();
        text << "\n\toutput.string rax" << endl;
        text << "\tpop rax";
//        text << node->getChild(0)->getLabel();
    } 
    if (type == TNUMBER) {
           node->getChild(0)->accept(this);
           text << "\toutput.int " << freeRegs.front() << endl;
    }
     if (type == TCHAR) {
           node->getChild(0)->accept(this);
           text << "\toutput.char " << freeRegs.front() << endl;
    }


    text << "\n";
}

void x86Visitor::visit(NReturn *node) {
    cerr << "Return Node" << endl;
	Node *retVal = node->getChild(0);
	retVal->accept(this);
}

void x86Visitor::visit(NStatementList *node) {
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}

void x86Visitor::visit(NStringLit *node) {
    /* Add strings to the .data section 
     * TODO: We still need to find a way
     * of labeling stringlits and their sizes
     *
     */
    string label = this->labelMaker.getNewLabel();
    data << label << ": db  " ;
    data << node->getID() <<",0x0" << endl; 
    node->setLabel(label);
}

void x86Visitor::visit(NUnaryOp *node) {
	cerr << "Node: Unary Op" << endl;
    node->getChild(0)->accept(this);
    string resultReg =  getNextReg();
    switch(node->getOp()) {
        case DASH:
            text << "\tneg " << resultReg << endl;           
			break;
        case LNOT:
 		case NOT:
            text << "\tnot " << resultReg << endl;           
        	break;
        case PLUS:
            break;
    }
	restoreStore(resultReg);
}
/* Variables are allocated on the stack, and are given
 * an address as [rbp +/- offset] TODO: Replace 4 with offset
 */
void x86Visitor::visit(NVariableDeclaration *node) {
    int type = node->getType();
    stringstream convert;
    switch(type) {
        case TNUMBER:
            /* reserve 4 bytes for an integer */
            offset += 8;
            text << "\tsub rsp, " << offset << endl;
             convert << "rbp-" << offset;
            node->setLabel(convert.str());
            break;
        case TCHAR:
            offset += 8; 
            text << "\tsub rsp, " << offset << endl;
            convert << "rbp-" << offset;
            node->setLabel(convert.str());           
            break;
    }
}

void x86Visitor::createSubroutine(string name) {
    text << "_" << name << ":\n";    
    text << "\tpush rbp\n";
    text << "\tmov rbp, rsp\n";
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

void x86Visitor::restoreCalleeReg() {
    /** Return from subroutine**/
    text << "pop rsi\n";
    text << "pop rdi\n";
    text << "pop rbx\n";
}
void x86Visitor::deallocVar() {

}
void x86Visitor::ret() {
    text << "\tmov rsp,rbp\n";
    text << "\tpop rbp\n";   
    text << "\tret\n";
}

string x86Visitor::getAssembly() {
    data << text.str();
    return data.str();
}

void x86Visitor::generateFunctionDefinitions()
{
	text << "\tcall _hattalabel1\n \tsys.exit\n";	
    while (!funcDecQueue.empty()) {
        cerr << "Generating Function Body for : " 
             << funcDecQueue.front()->getID();
        unfoldedFunctionVisitor(funcDecQueue.front());
        funcDecQueue.pop();
        
    }
}

void x86Visitor::unfoldedFunctionVisitor(NFunctionDeclaration* node) {
    cerr << "\nUnfolding: " << node->getID() << endl;
    /* Sets up our function label and base pointer*/
    createSubroutine(node->getLabel());
    int numChildren = node->getChildrenSize();
    
    if (numChildren == 2) {
        /* we have paramaters */
        /* TODO get dat stuff from the stack and put into regs son*/
        NParamDeclarationBlock* params = (NParamDeclarationBlock*) node->getChild(numChildren - 1);
        params->accept(this);
    }
    NCodeBlock* codeblock = (NCodeBlock*)node->getChild(0);
    NStatementList* statlist = (NStatementList*)codeblock->getChild(0);
    if (codeblock->getChildrenSize() == 2) {
        /* we have local variables*/
        NDeclarationBlock* decblock = (NDeclarationBlock*)codeblock->getChild(0);
        statlist = (NStatementList*)codeblock->getChild(1);
        /* reserve space on stack from them */
        /*TODO should probably get some way of total size of local vars*/
        /* ratheri than just assuming they are all 32bits */
        decblock->accept(this);
    } 
    statlist->accept(this);    
    this->ret();
}

void x86Visitor::init(Node* root) {
    /* initial call to set up our .data section*/
    data << "%include\t\'system.inc\'\n\n";
    data << "section .data\n\n";

    /* get all global variables and string literals used in text...*/
    boost::shared_ptr<SymbolTable> t = root->getTable();
    table_t::iterator it;

    string labelSuffix = this->labelMaker.getNewLabel();

    /* global vars, reserve space for em init" */
    for(it = t->start(); it != t->end(); it++) {
        string label = it->first + labelSuffix;
        data << "common\t" << label << "  " 
                << it->second->getSize() << endl;
        it->second->setLabel(label);
    }
 
    text << "section .text\n";
    text << "global _start\n\n";
    text << "_start:\n";
}

/* private helper functions */

string x86Visitor::getNextStore() {
    string res;
    if (freeRegs.size() == 1) {
        res = "[rsp]";
    } else {

        res = getNextReg();
    }
    return res;
}

string x86Visitor::getNextReg() {
    string res;
    if (freeRegs.size() == 1) {
        res = freeRegs.front();
    } else {
        res = freeRegs.front();
        freeRegs.pop_front();
    }
	cerr << "\t Using register: " << res << endl;
    return res;
}

void x86Visitor::restoreStore(string store) {
    if (find(allRegs.begin(), allRegs.end(), store) != allRegs.end()) {
        /* allRegs contains store */
		cerr << "\t Restoring register: " << store << endl;
        freeRegs.push_front(store);
    }
}

/* list of avalible registers for GP computation 
 * rax, rbx rcx, rdx, rdi, rsi, r8, r9, r10, r11, r12, r13, r14, r15,
 */
