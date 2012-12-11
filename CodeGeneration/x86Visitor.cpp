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
    /* We use rax to store return values, so it is not included here. */
    allRegs.clear();
    regsToRestore.clear();
    callStackRegs.clear();
    allRegs.push_back("r8");
    allRegs.push_back("r9");
    allRegs.push_back("r10");
    allRegs.push_back("r11");
    allRegs.push_back("r12");
    allRegs.push_back("r13");
    allRegs.push_back("r14");
    allRegs.push_back("r15");
    allRegs.push_back("rbx");
    allRegs.push_back("rcx");
    allRegs.push_back("rdx");
    allRegs.push_back("rsi");
    allRegs.push_back("rdi");
    freeRegs = allRegs;
}


void x86Visitor::printRegDeq(std::deque<string> regs) {
       
    std::deque<string>::iterator it = regs.begin();
    int i = 0;
    for (; it != regs.end(); it++) {
        cerr << "index[" << i << "]: " << *it << endl;
        i++;
    }
}

void x86Visitor::visit(NArrayAccess *node) {

}

void x86Visitor::visit(NAssignment *node) {
    cerr << "Node: Assignment" << endl;
    int rval = node->getChildrenSize() - 1;
    int lval = rval-1;

    node->getChild(rval)->accept(this);
    string reg = getNextReg();
    if (node->getChildrenSize() > 1) {
        Node* leftChildID = node->getChild(lval);
        Node* declaredNode = node->getTable()->lookup(leftChildID->getID());
        if (declaredNode->getLevel() == 1)
	       text << "\tmov [" <<  declaredNode->getLabel() << "]" ;      	
        else
	       text << "\tmov " <<  declaredNode->getLabel();
        text << ", " << reg <<  endl;
         //Assign the result to the variable
    }
    restoreStore(reg);
    cerr << "End: Assignment" << endl;
}
/* TODO: still need to implement a register saving mechanism 
 * maybe paramerterise visit with a list of reg's it can use
 * like the tutorial.
 */
void x86Visitor::generateBinOpInstr(int op, string returnReg, string nxtReg) {
    switch(op) {
        case OR:
        case LOR:
            text << "\tor " << returnReg << "," << nxtReg << endl;
            break;
        case AND:
        case LAND:
            text << "\tand " << returnReg << "," << nxtReg << endl;
            break;
        case LEQU:
            comparePredicate("sete", returnReg, nxtReg);    
            break; 
        case LLTHAN:
            comparePredicate("setl", returnReg, nxtReg);
            break;
        case LLTHANEQ:
            comparePredicate("setle", returnReg, nxtReg);
            break;
        case LGTHAN:
            comparePredicate("setg", returnReg, nxtReg);
            break;
        case LGTHANEQ:
            comparePredicate("setge", returnReg, nxtReg);
            break;
        case LNOTEQU:
            comparePredicate("setne", returnReg, nxtReg);
            break;
        case XOR:
            text << "\txor " << returnReg << ", " << nxtReg << endl;
            break;
        case PLUS:
            text << "\tadd " << returnReg << ", " << nxtReg << endl;
            break;
        case DASH:
            text << "\tsub " << returnReg << ", " << nxtReg << endl;
            break;
        case MULT:
            text << "\timul " << returnReg << ", " << nxtReg << endl;
            break;
        case DIV:
            text << "\tpush rax" << endl;
            text << "\tpush rdx" << endl;
            /* not sure if rax or rdx... see intel documentation */
            text << "\tmov " << "rax, " << returnReg << endl;
            text << "\tcqo" << endl;
            text << "\tidiv " << nxtReg << endl;
            /* quotient lives in rax, move to resultReg */
            text << "\tmov " << returnReg << ", rax" << endl;
            text << "\tpop rdx" << endl;
            text << "\tpop rax" << endl;
            break;
        case MOD:
            text << "\tpush rax" << endl;
            text << "\tpush rdx" << endl;
            /* not sure if rax or rdx... see intel documentation */
            text << "\tmov " << "rax, " << returnReg << endl;
            text << "\tcqo" << endl;
            text << "\tidiv " << nxtReg << endl;
            /* remainder lives in rdx, move to resultReg */
            text << "\tmov " << returnReg << ", rdx" << endl;
            text << "\tpop rdx" << endl;
            text << "\tpop rax" << endl;
            break;
    }
}
void x86Visitor::visit(NBinOp *node) {
    string resultReg, nxtReg;
    if (node->getChild(0)->getWeight() > node->getChild(1)->getWeight()) {
        cerr << "\nBinOp: evaluating  left first\nFree Regsiters:\n";
        printRegDeq(freeRegs);
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
    } else {
        cerr << "\nBinOp: evaluating right first\nFree Regsiters:\n";
        printRegDeq(freeRegs);
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
    }
    generateBinOpInstr(node->getOp(), resultReg, nxtReg);   
    restoreStore(nxtReg);
    restoreStore(resultReg); 
    cerr << "End: Bin Op " << endl;
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
    string reg = getNextReg();
    text << "\t mov " << reg << ", '" << node->getID() << "'\n"; 
    cerr << "End: Char Lit " << endl;
    restoreStore(reg);
}

void x86Visitor::visit(NCodeBlock *node) {
    /* Visit the statement list, and declaration list if it exists. */
    cerr << "Node: CodeBlock" << endl;
    text << "\tpush rbp" << endl;
    text << "\tpush rbp" << endl;
    text << "\tmov rbp, rsp" << endl;
    for(int i = 0; i < node->getChildrenSize(); ++i) {
        node->getChild(i)->accept(this);
    }
    text << "\tmov rsp, rbp" << endl;
    text << "\tpop rbp" << endl;
    text << "\tpop rbp" << endl;
    cerr << "End: CodeBlock" << endl;
}

void x86Visitor::visit(NConditional *node) {
    cerr << "Node: Coditional " << endl;
    bool startConditional = this->labelMaker.needsNewEndCondLabel();
    string endLabel = this->labelMaker.getEndCondLabel();
    string elseLabel = this->labelMaker.getNewLabel();

    // Visit the predicate to print out condition

    text << "; doing the comparison" << endl;   
 
    node->getChild(0)->accept(this);
    text << "\n; has free regs broken?" << endl;
    string resultReg = this->getNextReg();

    // If condition is false, jump to next condition

    text << "\n\tcmp " << resultReg << ", 1" << endl;
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
    cerr << "End: Coditional " << endl;
}

void x86Visitor::visit(NEndIf *node) {
    cerr << "Node: Endif" << endl;
    text << "\t;endif\n" << endl;
    cerr << "End: Endif" << endl;
}

/* NDeclarationBlock is our 'entry' point - its not exclusve though, ie every 
 * function has one
 */
void x86Visitor::visit(NDeclarationBlock *node) {
    cerr << "Node: Declaration Block" << endl;
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
     cerr << "End: Declaration Block" << endl;   
}

void x86Visitor::visit(NFunctionDeclaration *node) {
    if (node->getLevel() != 1) {
        string label = "_" + node->getID() + this->labelMaker.getNewLabel();
        node->setLabel(label);
    }
    funcDecQueue.push(node);
}

void x86Visitor::visit(NIdentifier *node) {
    cerr << "Node: Identifier" << endl;
    Node *declarationNode = node->getTable()->lookup(node->getID());
    int nestingLevel = declarationNode->getLevel();
    int numJumps = node->getLevel() - nestingLevel;
    string storeage = getNextReg();
    /* check if in local or global scope */
    if (nestingLevel == node->getLevel() || nestingLevel == 1) {
        /* just use the label */
        /* move from label into freeRegs.front() */
	if (nestingLevel == 1)
	        text << "\tmov " <<  storeage << ", [" << declarationNode->getLabel() <<"]" << endl;
	else
		text << "\tmov " <<  storeage << ", " << declarationNode->getLabel() << endl;

    } else {
        /* push rbp */
        text << "\t\n;Nesting level: " << nestingLevel << "\tThis Level : " << node->getLevel() << endl;
        text << "\tpush rbp" << endl;
        /* TODO :- write an assembly loop to stop generated code being bad */
        while(numJumps > 0) {
            /* follow the access link back overwriting rbp */
            text << "\tmov rbp, [rbp+16]" << endl;
            numJumps--;
        }
        /* mov from label into freeRegs.front() */
            text << "\tmov " << storeage << ", " << declarationNode->getLabel() << endl;
        /* pop rbp */
            text << "\tpop rbp" << endl;
    }
    restoreStore(storeage);
    cerr << "End: Identifier" << endl;
}

/* DOUBLE CHECK RESTORING REGS*/
void x86Visitor::visit(NInc *node) {
    cerr << "Node: Inc " << endl;

    /* Visit the node containing the expression to be incremented. */
    node->getChild(0)->accept(this);
    boost::shared_ptr<SymbolTable> table = node->getTable();
    string childID = node->getChild(0)->getID();
    Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
    string store = getNextStore();
    text << "\tinc " << store << "\n";
    text << "\tmov " << childDeclarationNode->getLabel() << ", "
         << store << endl;
    restoreStore(store);
    cerr << "End: Inc " << endl;
}

void x86Visitor::visit(NDec *node) {
    cerr << "Node: Decrement" << endl;
    /* Visit the node containing the expression to be decremented. */
    node->getChild(0)->accept(this);
    boost::shared_ptr<SymbolTable> table = node->getTable();
    string childID = node->getChild(0)->getID();
    Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
    string store = getNextStore();
    text << "\tdec " << store << "\n";
    text << "\tmov " << childDeclarationNode->getLabel() << ", "
         << store << endl;
    restoreStore(store);
    cerr << "End: Decrement" << endl;
}
/* TODO : - add input cases for other valid input types */
void x86Visitor::visit(NInput *node) {
    cerr << "Node: Input \n";
    Node* identifier = node->getChild(0);
    Node *var = node->getTable()->lookup(identifier->getID());
    string scanfstring = "scanf" + labelMaker.getNewLabel();
    string addr = var->getLabel();
    switch(var->getType()) {

        case TNUMBER:
            data << scanfstring << ": " << "db \"%d\", 0x0 " << endl;
            /* save registers */
            text << "\tpush rdi\n\tpush rax\n";
            /* mov params into rdi, rsi and rax, + zero out buffer
             * before calling scanf
             */ 
            text << "\tmov rdi, " << scanfstring << endl;
            text << "\tlea rsi , " << addr << "\n";
            text << "\tmov rax, 0\n";
            text << "\tmov qword " << addr << ", 0\n";
            text << "\tcall scanf" << endl;
            text << "\tmovsx rax, dword " << addr << "\n";
            text << "\tmov " << addr << ", rax\n";
            text << "\tpop rax" << endl;
            text << "\tpop rdi" << endl;
            break;
    }
    cerr << "End: Input" << endl;
}

void x86Visitor::visit(NInteger *node) {
    cerr << "Node: Integer" << endl;
    string store = getNextReg();
    text << "\n\tmov " << store << ", " << node->getValue();
    text << endl;
    restoreStore(store);
    cerr << "End: Integer" << endl;
}

void x86Visitor::visit(NLoop *node) {
    cerr << "Node: Loop " << endl;
    /* Get a label for the start and end of the while loop. Print start label.*/
    string startLabel = labelMaker.getNewLabel();
    string endLabel = labelMaker.getNewLabel();
    text << startLabel << ":" << endl;

    /* 
     * Visit condition. Assuming that 0 is stored in flags register if false,
     * jump to endLabel if the condition does not hold.
     */
    node->getChild(0)->accept(this);
    string reg = getNextReg();
    text << "\tcmp " << reg << ", 1" << endl;
    text << "\tjne " << endLabel << "\n";
    
    /* Visit the statement list of the loop node, then recheck condition. */
    node->getChild(1)->accept(this);
    text << "\tjmp " << startLabel << "\n";

    /* Print end label finish. */
    text << endLabel << ":\n";
    cerr << "End: Loop" << endl;
}

void x86Visitor::visit(NMethodCall *node) {
     cerr << "Node: Function or Procedure Call" << endl;   
    /* Push paramaters onto the stack in reverse order */
    pushRegs();
    if (node->getChildrenSize() > 0) {
        node->getChild(0)->accept(this);
    } 
    /* Save registers currently in use */

    Node* MethodDec = node->getTable()->lookup(node->getID());
    /* Figure out if MethodDec is nested within current function */
    /* update the access link -> always resides at rbp + 16 */
    /* and pass as a paramter to the function */
    text << "\tpush rbp" << endl;
 
    /* call the method */
    text << "\tcall _" << MethodDec->getLabel() << endl;
    /* Remove the access link from the stack */
    text << "\tadd rsp, 8" << endl;
    if (node->getChildrenSize() > 0 ) {
    	    int numParams = node->getChild(0)->getChildrenSize();
	    text << "\tadd rsp, " << numParams * 8 << endl;
    }

    /* Restore registers we were using */
    popRegs();
    /* TODO: - Procedure doesn't need to save return value */
    /*save return value from rax into freeRegisters.front()*/
    string reg = getNextReg();
    text << "\tmov " << reg << ", rax" << endl;
    restoreStore(reg);
     
     cerr << "End: Function or Procedure Call" << endl;   
}

void x86Visitor::visit(NNullToken *node) {
    cerr << "Node: NNullToken" << endl;
    text << "\tnop" << endl;
    cerr << "End: NullToken" << endl;
}

void x86Visitor::visit(Node *node) {

}

/* puts paramtaters into registers or stack */
void x86Visitor::visit(NParamBlock *node) {
    cerr << "Node: Param Block " << endl;      
    unsigned int numChildren =  node->getChildrenSize();
    string label;
    std::deque<string>::iterator it = allRegs.begin();
    unsigned int i = 0;

    /* start moving paramters into regsiters 
    for(it = allRegs.begin(); it != allRegs.end() && i < numChildren; it++) {
            //            cerr << "Paramater passed in : " << res << endl;
	
        node->getChild(i)->accept(this);
	string res = getNextReg();
	text << "mov  " << (*it) <<  ", " << res;
            
            i++;
    }*/
    /* if we run out of register for paramaters (GOD WHY?) use the stack*/   
    for (; i < numChildren; i++) {
          node->getChild(i)->accept(this);
          text << "\t; push child node" << endl;
          text << "\tpush " << getNextReg() << endl;
    }
    printRegDeq(freeRegs);
    cerr << "End: Param Block " << endl;
}
/* labels paramaters as in registers or on stack */
void x86Visitor::visit(NParamDeclarationBlock *node) {
    cerr << "Node: Param Declaration block " << endl;
    /* offset to first param starts at 24:
     *      rbp         +  8
     *      ret         +  8
     *      AccessLink  +  8
     *                 ------
     *      (offset)    = 24
     *                 ------
     *                    2
     */
    unsigned int numChildren = node->getChildrenSize();
    unsigned int offset = 24;

    string label;
    std::deque<string>::iterator it = allRegs.begin();
    unsigned int i = 0;
    /* start labeling paramters as using regsiters 
    for(it = allRegs.begin(); it != allRegs.end() && i < numChildren; it++) {
        node->getChild(i)->setLabel(*it);
        i++;
        std::deque<string>::iterator element =  find(freeRegs.begin(),freeRegs.end(),*it);
        if (element != freeRegs.end()) {
            freeRegs.erase(element);
        }
        
    }*/
    printRegDeq(freeRegs);
    /* if we run out of registers to put params in start using the stack */
    for (; i < numChildren; i++) {
        label = "[rbp+";
        label = label + boost::lexical_cast<string>(offset) + "]";
        node->getChild(i)->setLabel(label);
        offset+=8;
    }
    cerr << "End: Param Declaration block " << endl;   
}
/*TODO: change this from output char to output string or something..*/
void x86Visitor::visit(NPrint *node) {
    cerr << "Node: Print" << endl;
    int type,nodeType;
    nodeType = node->getChild(0)->getNodeType();
    type = node->getChild(0)->getType();
    string printlabel = "prnt" + labelMaker.getNewLabel() ;
    if(nodeType == CHARLIT) {
    /* We can use the macro defed in system.inc */
        text << "output.char ";
        node->getChild(0)->accept(this);   
    }
    if(nodeType == STRINGLIT) {
        node->getChild(0)->accept(this);   
        text << "\tpush rax\n\tmov rax," << node->getChild(0)->getLabel();
        text << "\n\toutput.string rax" << endl;
        text << "\tpop rax" << endl;
//        text << node->getChild(0)->getLabel();
    }
    if (type == TNUMBER) {
        node->getChild(0)->accept(this);
        string reg = getNextReg();
        data << printlabel << ": db \"%d\" , 0x0 " << endl;
	text << "\tpush r8" << endl;
        text << "\tpush rax\n\tpush rdi" << endl;
        text << "\tpush rsi\n";
        text << "\tmov rsi," << reg << endl;
        text << "\tmov rdi, " << printlabel << endl;
        text << "\tmov rax, 0" << endl;
        text << "\tcall printf" << endl;
        text << "\tpop rsi\n\tpop rdi\n\tpop rax\n";
	text << "\tpop r8" << endl;
        restoreStore(reg);
    }
    if (type == TCHAR) {
           node->getChild(0)->accept(this);
           string reg = getNextReg();
           text << "\toutput.char " << reg << endl;
           restoreStore(reg);
    }
    if(type == TSTRING) {
        node->getChild(0)->accept(this);   
	Node* decnode = node->getTable()->lookup(node->getChild(0)->getID());
	if (decnode != NULL) {
	        text << "\tpush rax\n\tmov rax," << decnode->getLabel();
	        text << "\n\toutput.string rax" << endl;
	        text << "\tpop rax" << endl;
	}
//        text << node->getChild(0)->getLabel();
    }
    cerr << "Node Type: " << nodeType << "\tType: " << type << endl;

    text << "\n";
    cerr << "End Print" << endl;
}
/* RAX is used as the 'return register' */
void x86Visitor::visit(NReturn *node) {
    cerr << "Node: Return" << endl;
    Node *retVal = node->getChild(0);
    retVal->accept(this);
    string reg = getNextReg();
    text << "mov rax, " << reg << endl;
    restoreStore(reg);
    cerr << "End: Return" << endl;   
    ret();
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
    cerr << "Node: String Literal" << endl;
    string label = this->labelMaker.getNewLabel();
    data << label << ": db  " ;
    data << node->getID() <<",0x0" << endl; 
    node->setLabel(label);
    string reg = getNextReg();
    text << "\tmov " << reg << ", " << label << endl;
    restoreStore(reg);
    cerr << "End: String Literal" << endl;
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
    cerr << "End: Unary Op" << endl;
}
/* Variables are allocated on the stack, and are given
 * an address as [rbp +/- offset] TODO: Replace 4 with offset
 */
void x86Visitor::visit(NVariableDeclaration *node) {
    cerr << "\nNode: VarDec" << endl;
    int type = node->getType();
    stringstream convert;
    switch(type) {
        case TNUMBER:
            /* reserve 4 bytes for an integer */

        case TCHAR:
            offset += 8; 
            text << "\tsub rsp, " << 8 << endl;
            convert << "[rbp-" << offset << "]";
            node->setLabel(convert.str());           
            break;
        default:
            node->setLabel("broken :( ");
    }
    cerr << "\n End: VarDec" << endl;
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

void x86Visitor::ret() {
    text << "\tmov rsp,rbp\n";
    text << "\tpop rbp\n";   
//    text << "\tmov rax, " << freeRegs.front() << endl;
    text << "\tret\n";
}

string x86Visitor::getAssembly() {
    data << text.str();
    return data.str();
}

void x86Visitor::generateFunctionDefinitions()
{
    text << "\tcall _hattalabel1" << endl;
    /* Move EXIT_SUCCESS into rdi and call exit() */
    text << "\tmov rdi, " << EXIT_SUCCESS << endl;
    text << "\tcall exit" << endl;
    
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
    freeRegs = allRegs;
	    this->ret();
    cerr << "\n*Unfolded: " << node->getID() << "*" << endl;
    printRegDeq(freeRegs);
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
    text << "global main\n";
    text << "extern exit\n";
    text << "extern scanf\n";
    text << "extern printf\n";
    text << "main:\n";
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
        cerr << "\t Removing register from list: " << res << endl;
    }
//    cerr << "\t Using register: " << res << endl;
    return res;
}

void x86Visitor::restoreStore(string store) {
    if (find(allRegs.begin(), allRegs.end(), store) != allRegs.end()) {
        /* allRegs contains store */
        cerr << "\t Restoring register: " << store << endl;
        freeRegs.push_front(store);
    }

}

void x86Visitor::restoreStore() {
    for (unsigned int i = 0; i < regsToRestore.size(); i++) {
        restoreStore(regsToRestore[i]);
        
    }
    regsToRestore.clear();
}


/* list of avalible registers for GP computation 
 * rax, rbx rcx, rdx, rdi, rsi, r8, r9, r10, r11, r12, r13, r14, r15,
 */


/* pushes registers that are 'in use' */
void x86Visitor::pushRegs() {
    cerr << "\t\t**Pushing regsiters in use**" << endl;
    std::deque<string>::iterator it;
    for (it = allRegs.begin(); it != allRegs.end(); it++ ) {
        if (find(freeRegs.begin(),freeRegs.end(),(*it)) == freeRegs.end())
            text << "\tpush " << (*it) << endl;
    }
    callStackRegs.push_front(freeRegs);
    freeRegs = allRegs;
    cerr << "\t\t**New freeRegs deque**" << endl;
    printRegDeq(freeRegs);
}

/* Pops registers that are 'in use' */
void x86Visitor::popRegs() {
    freeRegs = callStackRegs.front();
    callStackRegs.pop_front();

    std::deque<string>::iterator it;
    for (it = allRegs.begin(); it != allRegs.end(); it++ ) {
        if (find(freeRegs.begin(),freeRegs.end(),(*it)) == freeRegs.end())
            text << "\tpop " << (*it) << endl;
    }

}
