#include "GenericASTVisitor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "../Node/NodeIncludes.hpp"
#include "x86CodeGenerator.hpp"

/* Register conventions -
 * RAX stores the result of any procedure or function
 * a function can have an arbitrary number of paramters which are stored on the
 * stack in reverse order
 */

string convertInt(int);


GenericASTVisitor::GenericASTVisitor() : labelMaker() {
    offset = 0;
}


void GenericASTVisitor::printRegDeq(std::deque<string> regs) {
       
    std::deque<string>::iterator it = regs.begin();
    int i = 0;
    for (; it != regs.end(); it++) {
        cerr << "index[" << i << "]: " << *it << endl;
        i++;
    }
}

void GenericASTVisitor::visit(NArrayAccess *node) {
    cerr << "Node: Array Access" << endl;
    Node* offsetExp = node->getChild(1);
    Node* arrName = node->getTable()->lookup(node->getChild(0)->getID());
    offsetExp->accept(this);
    string offset = getNextReg();
    string addrReg = getNextReg();
    string result = getNextReg();

	generator->generateAccessInstruction(addrReg, offset, result,
												arrName->getLabel());

	restoreStore(offset);
	restoreStore(addrReg);
	restoreStore(result);

    cerr << "End: Array Access" << endl;   

}

void GenericASTVisitor::visit(NAssignment *node) {
			cerr << "Node: Assignment" << endl;
			int rval = node->getChildrenSize() - 1;
			int lval = rval-1;
			string storeAddr;
			node->getChild(rval)->accept(this);
			string value = getNextReg();
			if (node->getChildrenSize() > 1) {
				Node* leftChildID = node->getChild(lval);
				Node* declarationNode = node->getTable()->lookup(leftChildID->getID());
				int nestingLevel = declarationNode->getLevel();
				int numJumps = node->getLevel() - nestingLevel;
				string label = declarationNode->getLabel();

				if (declarationNode->getType() == REFNUMBER ||
					declarationNode->getType() == REFCHAR) {
					leftChildID->accept(this);
					storeAddr = getNextReg();
					generator->printInstruction(AMOVE, "[" + storeAddr + "]",
															value);
				} else if (declarationNode->getType() == TSTRING)
					{
						declarationNode->setLabel(node->getChild(rval)->getLabel());
					}
				else {
				/* check if in local or global scope */
					if (nestingLevel == node->getLevel() || nestingLevel == 1) {
							if (nestingLevel == 1)
								label = "[" + label + "]";
							generator->printInstruction(AMOVE, label, value);
					} else {
						/* push rbp */
						generator->printInstruction(APUSH, generator->getBasePointer());
					/* TODO :- write an assembly loop to stop generated code being bad */
						while(numJumps > 0) {
						/* follow the access link back overwriting rbp */
							string bp = generator->getBasePointer();
							generator->printInstruction(AMOVE, bp, "[" + bp + "+16" + "]");
							numJumps--;
						}
						/* mov from label into freeRegs.front() */
						generator->printInstruction(AMOVE, label, value);
						/* pop rbp */
						generator->printInstruction(APOP, generator->getBasePointer());
				}
			}}
			restoreStore(storeAddr);
			restoreStore(value);
			
			cerr << "End: Assignment" << endl;
		}
		/* TODO: still need to implement a register saving mechanism 
		 * maybe paramerterise visit with a list of reg's it can use
		 * like the tutorial.
		 */
		void GenericASTVisitor::generateBinOpInstr(int op, string returnReg, string nxtReg) {
			switch(op) {
				case OR:
				case LOR:
					generator->printInstruction(AOR, returnReg, nxtReg);
					break;
				case AND:
				case LAND:
					generator->printInstruction(AAND, returnReg, nxtReg);
					break;
				case LEQU:
					generator->comparePredicate(SETE, returnReg, nxtReg); 
					break; 
				case LLTHAN:
					generator->comparePredicate(SETL, returnReg, nxtReg); 
					break;
				case LLTHANEQ:
					generator->comparePredicate(SETLE, returnReg, nxtReg); 
					break;
				case LGTHAN:
					generator->comparePredicate(SETG, returnReg, nxtReg); 
					break;
				case LGTHANEQ:
					generator->comparePredicate(SETGE, returnReg, nxtReg); 
					break;
				case LNOTEQU:
					generator->comparePredicate(SETNE, returnReg, nxtReg); 
					break;
				case XOR:
					generator->printInstruction(AXOR, returnReg, nxtReg);
					break;
				case PLUS:
					generator->printInstruction(AADD, returnReg, nxtReg);
					break;
				case DASH:
					generator->printInstruction(ASUB, returnReg, nxtReg);
					break;
				case MULT:
					generator->printInstruction(AIMUL, returnReg, nxtReg);
					break;
				case DIV:
					generator->printInstruction(ADIV, returnReg, nxtReg);
					break;
				case MOD:
					generator->printInstruction(AMOD, returnReg, nxtReg);
			break;
    }
}
void GenericASTVisitor::visit(NBinOp *node) {
    string resultReg, nxtReg;
    if (node->getChild(0)->getWeight() > node->getChild(1)->getWeight()) {
        printRegDeq(freeRegs);
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
    } else {
        printRegDeq(freeRegs);
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
    }
	
	if (node->getChild(0)->getNodeType() == ARRAYACCESS)
	{
		generator->printInstruction(AMOVE, resultReg, "[" + resultReg + "]");
	}
	if (node->getChild(1)->getNodeType() == ARRAYACCESS)
	{
		generator->printInstruction(AMOVE, nxtReg, "[" + nxtReg + "]");	
	}
    generateBinOpInstr(node->getOp(), resultReg, nxtReg);   
    restoreStore(nxtReg);
    restoreStore(resultReg); 
    cerr << "End: Bin Op " << endl;
}

/* TODO: As we have an output charlit macro I'm sure that we
 * no longer need to store this in the data segment
 * i.e the only reason we wanted to store it in .data was
 * because we didnt have a way of printing it before
 */
void GenericASTVisitor::visit(NCharLit *node) {
    cerr << "Node: Char Lit " << endl;
    string reg = getNextReg();
	generator->printInstruction(AMOVE, reg, "'" + node->getID() + "'");
    cerr << "End: Char Lit " << endl;
    restoreStore(reg);
}

void GenericASTVisitor::visit(NCodeBlock *node) {
    /* Visit the statement list, and declaration list if it exists. */
    cerr << "Node: CodeBlock" << endl;
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(APUSH, generator->getBasePointer());

	generator->printInstruction(AMOVE, generator->getBasePointer(),
										generator->getStackPointer());
    
	for(int i = 0; i < node->getChildrenSize(); ++i) {
        node->getChild(i)->accept(this);
    }
	generator->printInstruction(AMOVE, generator->getStackPointer(),
										generator->getBasePointer());
	
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
    cerr << "End: CodeBlock" << endl;
}

void GenericASTVisitor::visit(NConditional *node) {
    cerr << "Node: Coditional " << endl;
    bool startConditional = this->labelMaker.needsNewEndCondLabel();
    string endLabel = this->labelMaker.getEndCondLabel();
    string elseLabel = this->labelMaker.getNewLabel();

    // Visit the predicate to print out condition

    node->getChild(0)->accept(this);
    string resultReg = this->getNextReg();

    // If condition is false, jump to next condition

	generator->printInstruction(ACMP, resultReg, "1");
	generator->printInstruction(AJNE, elseLabel);    
    restoreStore(resultReg);

    // Otherwise, execute the statement list, first pushing
    // the end condition label onto the stack
    this->labelMaker.pushEndCondLabel();
    node->getChild(1)->accept(this);

    this->labelMaker.popEndCondLabel();

    // Return to end of conditional
    generator->printInstruction(AJMP, endLabel);

    // Print out code for rest of cases
	generator->printLabel(elseLabel);
    node->getChild(2)->accept(this);


    if (startConditional)  {
		generator->printLabel(endLabel);
        this->labelMaker.resetEndCondLabel();
    }
    cerr << "End: Coditional " << endl;
}

void GenericASTVisitor::visit(NEndIf *node) {
}

/* NDeclarationBlock is our 'entry' point - its not exclusve though, ie every 
 * function has one
 */
void GenericASTVisitor::visit(NDeclarationBlock *node) {
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

void GenericASTVisitor::visit(NFunctionDeclaration *node) {
    if (node->getLevel() != 1) {
        string label = "_" + node->getID() + this->labelMaker.getNewLabel();
        node->setLabel(label);
    }
    funcDecQueue.push(node);
}

void GenericASTVisitor::visit(NIdentifier *node) {
    cerr << "Node: Identifier" << endl;
    Node *declarationNode = node->getTable()->lookup(node->getID());
    int nestingLevel = declarationNode->getLevel();
    int numJumps = node->getLevel() - nestingLevel;
    string storage = getNextReg();
    /* check if in local or global scope */
    if (nestingLevel == node->getLevel() || nestingLevel == 1) {
        /* just use the label */
        /* move from label into freeRegs.front() */
	if (nestingLevel == 1)
			generator->printInstruction(AMOVE, 
					storage, "[" + declarationNode->getLabel() + "]");
	else
			generator->printInstruction(AMOVE, storage,
					declarationNode->getLabel());
    } else {
        generator->printInstruction(APUSH, generator->getBasePointer());
        /* TODO :- write an assembly loop to stop generated code being bad */
        while(numJumps > 0) {
            /* follow the access link back overwriting rbp */
			string bp = generator->getBasePointer();
            generator->printInstruction(AMOVE, bp, "[" + bp + "+16" + "]");
            numJumps--;
        }
        /* mov from label into freeRegs.front() */
			generator->printInstruction(AMOVE, storage,
				declarationNode->getLabel());
        /* pop rbp */
			generator->printInstruction(APOP, generator->getBasePointer());
    }
    restoreStore(storage);
    cerr << "End: Identifier" << endl;
}

/* DOUBLE CHECK RESTORING REGS*/
void GenericASTVisitor::visit(NInc *node) {
    cerr << "Node: Inc " << endl;

    /* Visit the node containing the expression to be incremented. */
    node->getChild(0)->accept(this);
    boost::shared_ptr<SymbolTable> table = node->getTable();
    string childID = node->getChild(0)->getID();
    Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
    string store = getNextStore();
	generator->printInstruction(AINC, store);
    generator->printInstruction(AMOVE, childDeclarationNode->getLabel(), store);
    restoreStore(store);
    cerr << "End: Inc " << endl;
}

void GenericASTVisitor::visit(NDec *node) {
    cerr << "Node: Decrement" << endl;
    /* Visit the node containing the expression to be decremented. */
    node->getChild(0)->accept(this);
    boost::shared_ptr<SymbolTable> table = node->getTable();
    string childID = node->getChild(0)->getID();
    Node *childDeclarationNode = table->lookup(childID);
    /* Increment the register containing the result of the expression */
    string store = getNextStore();
	generator->printInstruction(ADEC, store);
    generator->printInstruction(AMOVE, childDeclarationNode->getLabel(), store);
    restoreStore(store);
    cerr << "End: Decrement" << endl;
}
/* TODO : - add input cases for other valid input types */
void GenericASTVisitor::visit(NInput *node) {
    cerr << "Node: Input \n";
    Node* identifier = node->getChild(0);
    Node *var = node->getTable()->lookup(identifier->getID());
    string scanfstring = "scanf" + labelMaker.getNewLabel();
    string addr; 
    if (var->getLevel() == 1)
	    addr = "[" + var->getLabel() + "]" ;
    else
    	 addr = var->getLabel();
    switch(var->getType()) {
        case TNUMBER:
			generator->generateInputFunction(scanfstring, addr);
            break;
    }
    cerr << "End: Input" << endl;
}

void GenericASTVisitor::visit(NInteger *node) {
    cerr << "Node: Integer" << endl;
    string store = getNextReg();
	generator->printInstruction(AMOVE, store, convertInt(node->getValue()));
    restoreStore(store);
    cerr << "End: Integer" << endl;
}

void GenericASTVisitor::visit(NLoop *node) {
    cerr << "Node: Loop " << endl;
    /* Get a label for the start and end of the while loop. Print start label.*/
    string startLabel = labelMaker.getNewLabel();
    string endLabel = labelMaker.getNewLabel();
    generator->printLabel(startLabel);

    /* 
     * Visit condition. Assuming that 0 is stored in flags register if false,
     * jump to endLabel if the condition does not hold.
     */
    node->getChild(0)->accept(this);
    string reg = getNextReg();
	generator->printInstruction(ACMP, reg, "1");
    generator->printInstruction(AJE, endLabel);
    
    /* Visit the statement list of the loop node, then recheck condition. */
    node->getChild(1)->accept(this);
   	generator->printInstruction(AJMP, startLabel);
    /* Print end label finish. */
    generator->printLabel(endLabel);
    cerr << "End: Loop" << endl;
}

void GenericASTVisitor::visit(NMethodCall *node) {
     cerr << "Node: Function or Procedure Call" << endl;   
    /* Push paramaters onto the stack in reverse order */
    pushRegs();
    if (node->getChildrenSize() > 0) {
        node->getChild(0)->accept(this);
    } 
    /* Save registers currently in use */

    Node* MethodDec = node->getTable()->lookup(node->getID());
    
	int numJumps = node->getLevel() - MethodDec->getLevel();

	string freeReg = getNextReg();

	generator->printInstruction(AMOVE, freeReg, generator->getBasePointer());
	while (numJumps > 0)
	{
		generator->printInstruction(AMOVE, freeReg, "[" + freeReg + "+16]");
		numJumps--;
	}

	generator->printInstruction(APUSH, freeReg); 
    /* call the method */
	generator->printInstruction(ACALL, "_" + MethodDec->getLabel());
	restoreStore(freeReg);
    /* Remove the access link from the stack */
	generator->printInstruction(AADD, generator->getStackPointer(), "8");    
    if (node->getChildrenSize() > 0 ) {
    	    int numParams = node->getChild(0)->getChildrenSize();
		generator->printInstruction(AADD, generator->getStackPointer(),
							convertInt(numParams * 8));	
    }

    /* Restore registers we were using */
    popRegs();
    string reg = getNextReg();
    generator->printInstruction(AMOVE, reg, generator->getReturnRegister());
    restoreStore(reg);
     cerr << "End: Function or Procedure Call" << endl;   
}

void GenericASTVisitor::visit(NNullToken *node) {
}

void GenericASTVisitor::visit(Node *node) {

}

/* puts paramtaters into registers or stack */
void GenericASTVisitor::visit(NParamBlock *node) {
    cerr << "Node: Param Block " << endl;      
    unsigned int numChildren =  node->getChildrenSize();
    string label;
    std::deque<string>::iterator it = allRegs.begin();
    /* push params onto the stack*/   
    for (int i = numChildren - 1; i >= 0; i--) {
        node->getChild(i)->accept(this);
		string reg = getNextReg();
		if (node->getChild(i)->getNodeType() == ARRAYACCESS) {
			generator->printInstruction(AMOVE, reg, "[" + reg + "]");	
		}
        generator->printInstruction(APUSH, reg);  
    }
    printRegDeq(freeRegs);
    cerr << "End: Param Block " << endl;
}
/* labels paramaters as in registers or on stack */
void GenericASTVisitor::visit(NParamDeclarationBlock *node) {
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
    printRegDeq(freeRegs);
    /* put params onto the stack */
    for (; i < numChildren; i++) {
		label = "[" + generator->getBasePointer() + "+";
        label = label + boost::lexical_cast<string>(offset) + "]";
        node->getChild(i)->setLabel(label);
        offset+=8;
    }
    cerr << "End: Param Declaration block " << endl;   
}
/*TODO: change this from output char to output string or something..*/
void GenericASTVisitor::visit(NPrint *node) {
    cerr << "Node: Print" << endl;
    int type,nodeType;
    nodeType = node->getChild(0)->getNodeType();
    type = node->getChild(0)->getType();
    Node* decNode = node->getTable()->lookup(node->getChild(0)->getID());
    int decType = 0;
    if (decNode != NULL)
        decType = decNode->getType();
    string printlabel = "prnt" + labelMaker.getNewLabel();
    if(nodeType == STRINGLIT) {
        node->getChild(0)->accept(this);   
		generator->generatePrintInstruction("", node->getChild(0)->getLabel(),
										 false, nodeType);
    }
    if (type == TNUMBER && decType != REFNUMBER) {
        node->getChild(0)->accept(this);
        string reg = getNextReg();
        generator->generatePrintInstruction(reg, printlabel, false, type);
		restoreStore(reg);
    }
    if (type == TCHAR && decType != REFCHAR) {
           node->getChild(0)->accept(this);
           string reg = getNextReg();
		   generator->generatePrintInstruction(reg, printlabel, false, type);
		   restoreStore(reg);
    }
	if (decType == REFCHAR || decType == REFNUMBER)
	{
		node->getChild(0)->accept(this);
		string reg = getNextReg();
		generator->generatePrintInstruction(reg, printlabel, true, type);
		restoreStore(reg);
    }
    if(type == TSTRING) {
      	node->getChild(0)->accept(this);   
		string reg = getNextReg();
	if (decNode != NULL) {
				generator->generatePrintInstruction(decNode->getLabel(),
										printlabel, false, type);
	}
		restoreStore(reg);
    }
    cerr << "Node Type: " << nodeType << "\tType: " << type << endl;
    cerr << "End Print" << endl;
}
/* RAX is used as the 'return register' */
void GenericASTVisitor::visit(NReturn *node) {
    cerr << "Node: Return" << endl;
    Node *retVal = node->getChild(0);
    retVal->accept(this);
    string reg = getNextReg();
	generator->printInstruction(AMOVE, generator->getReturnRegister(), reg);
    restoreStore(reg);
    cerr << "End: Return" << endl;   
    ret();
}

void GenericASTVisitor::visit(NStatementList *node) {
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}

void GenericASTVisitor::visit(NStringLit *node) {
    /* Add strings to the .data section 
     * TODO: We still need to find a way
     * of labeling stringlits and their sizes
     *
     */
    cerr << "Node: String Literal" << endl;
    string label = this->labelMaker.getNewLabel();
	generator->printStringLitData(label, node->getID()); 
    node->setLabel(label);
    string reg = getNextReg();
	generator->printInstruction(AMOVE, reg, label);
    restoreStore(reg);
    cerr << "End: String Literal" << endl;
}

void GenericASTVisitor::visit(NUnaryOp *node) {
    cerr << "Node: Unary Op" << endl;
    node->getChild(0)->accept(this);
    string resultReg =  getNextReg();
    switch(node->getOp()) {
        case DASH:
			generator->printInstruction(ANEG, resultReg);
            break;
        case LNOT:
        case NOT:
			generator->printInstruction(ANOT, resultReg);
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
void GenericASTVisitor::visit(NVariableDeclaration *node) {
    cerr << "\nNode: VarDec" << endl;
    int type = node->getType();
   stringstream debuglabel ;
	stringstream numElementsStr;
	Node *lengthNode;
	string label;
    int numElements; 
    stringstream convert;
    switch(type) {
        case TNUMBER:
            /* reserve 8 bytes for an integer */
        case TCHAR:
            offset += 8;
			generator->printInstruction(ASUB, 
							generator->getStackPointer(), "8"); 
			convert << "[" << generator->getBasePointer() << "-" << offset << "]";
	    if (node->getLevel() != 1)
	        node->setLabel(convert.str());           
		break;
    	case REFCHAR:
    	case REFNUMBER:
            lengthNode = node->getChild(0);
			if (lengthNode->getNodeType() == INTEGER) {
				numElements = ((NInteger*)(lengthNode))->getValue();
				numElements = numElements * 8;
				numElementsStr << numElements;
			} else {
				numElementsStr.clear();
				numElementsStr << node->getTable()->
						lookup(lengthNode->getID())->getLabel();
			}
			label = "arr" + labelMaker.getNewLabel();
			generator->printHeapAllocationInstruction(label,
										numElementsStr.str());	
			/* reserve numelements * 8 bytes worth of space on the stack */
            /* set the label to be a pointer to the offest of the 1st element */
            convert << "[" << label << "]";
            /* set the offest to take into account the entire array */
			if (lengthNode->getNodeType() == INTEGER) {
				offset+= numElements;
			}
            node->setLabel(convert.str());
        break;
        case TSTRING:
            
        break;
        default:
            debuglabel << type;
            node->setLabel(debuglabel.str());
    }
    cerr << "\n End: VarDec" << endl;
}

void GenericASTVisitor::createSubroutine(string name) {
	generator->printLabel("_" + name);
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(AMOVE, generator->getBasePointer(),
								generator->getStackPointer());    
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

void GenericASTVisitor::ret() {
	generator->printInstruction(AMOVE, generator->getStackPointer(), 
										generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(ARET);
}

string GenericASTVisitor::getAssembly() {
	return generator->getAssembly();
}

void GenericASTVisitor::generateFunctionDefinitions()
{
	generator->printInstruction(ACALL, "_hattalabel1");
    /* Move EXIT_SUCCESS into rdi and call exit() */
	generator->printReturnFromProgram();
    
    while (!funcDecQueue.empty()) {
        cerr << "Generating Function Body for : " 
             << funcDecQueue.front()->getID();
        unfoldedFunctionVisitor(funcDecQueue.front());
        funcDecQueue.pop();
        
    }
}

void GenericASTVisitor::unfoldedFunctionVisitor(NFunctionDeclaration* node) {
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

void GenericASTVisitor::init(Node* root, CodeGenerator* gen) {
    /* initial call to set up our .data section*/
	generator = gen;
	freeRegs.clear();
    allRegs.clear();
    callStackRegs.clear();
	allRegs = this->generator->getAllGeneralRegs();
    freeRegs = allRegs;

	generator->printInitial();

    /* get all global variables and string literals used in ext...*/
    boost::shared_ptr<SymbolTable> t = root->getTable();
    table_t::iterator it;

    string labelSuffix = this->labelMaker.getNewLabel();

    /* global vars, reserve space for em init" */
    for(it = t->start(); it != t->end(); it++) {
        string label = it->first + labelSuffix;
		generator->printData(label, convertInt(it->second->getSize()));
        it->second->setLabel(label);
    }

	generator->printExtern(); 
}

/* private helper functions */

string GenericASTVisitor::getNextStore() {
    string res;
    if (freeRegs.size() == 1) {
		res = "[" + generator->getStackPointer() + "]";
    } else {
        res = getNextReg();
    }
    return res;
}

string GenericASTVisitor::getNextReg() {
    string res;
    if (freeRegs.size() == 1) {
        res = freeRegs.front();
    } else {
        res = freeRegs.front();
        freeRegs.pop_front();
        cerr << "\t Removing register from list: " << res << endl;
    }
    return res;
}

void GenericASTVisitor::restoreStore(string store) {
    if (find(allRegs.begin(), allRegs.end(), store) != allRegs.end()) {
        /* allRegs contains store */
        cerr << "\t Restoring register: " << store << endl;
        freeRegs.push_front(store);
    }

}

void GenericASTVisitor::restoreStore() {
    for (unsigned int i = 0; i < regsToRestore.size(); i++) {
        restoreStore(regsToRestore[i]);
        
    }
    regsToRestore.clear();
}


/* list of avalible registers for GP computation 
 * rax, rbx rcx, rdx, rdi, rsi, r8, r9, r10, r11, r12, r13, r14, r15,
 */


/* pushes registers that are 'in use' */
void GenericASTVisitor::pushRegs() {
    cerr << "\t\t**Pushing regsiters in use**" << endl;
    std::deque<string>::iterator it;
    for (it = allRegs.begin(); it != allRegs.end(); it++ ) {
        if (find(freeRegs.begin(),freeRegs.end(),(*it)) == freeRegs.end())
            generator->printInstruction(APUSH, (*it));
    }
    callStackRegs.push_front(freeRegs);
    freeRegs = allRegs;
    cerr << "\t\t**New freeRegs deque**" << endl;
    printRegDeq(freeRegs);
}

/* Pops registers that are 'in use' */
void GenericASTVisitor::popRegs() {
    freeRegs = callStackRegs.front();
    callStackRegs.pop_front();

    std::deque<string>::iterator it;
    for (it = allRegs.begin(); it != allRegs.end(); it++ ) {
        if (find(freeRegs.begin(),freeRegs.end(),(*it)) == freeRegs.end())
    		generator->printInstruction(APOP, (*it));
    }

}

string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
