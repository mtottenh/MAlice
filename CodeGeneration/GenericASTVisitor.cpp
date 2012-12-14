#include "GenericASTVisitor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "../Node/NodeIncludes.hpp"
#include "x86CodeGenerator.hpp"

string convertInt(int);

GenericASTVisitor::GenericASTVisitor() : labelMaker() {
    offset = 0;
}

void GenericASTVisitor::visit(NArrayAccess *node) {
	Node* offsetExp = node->getChild(1);
    Node* arrName = node->getTable()->lookup(node->getChild(0)->getID());
    /*
	 * Evaluate the offset and get its value from the register at the
	 * front of the free list.
	 */
	offsetExp->accept(this);
    string offset = getNextReg();
    string addrReg = getNextReg();
    string result = getNextReg();

	/*
	 * Delegate to the code generator to create an array access 
	 * instruction 
	 */
	generator->generateAccessInstruction(addrReg, offset, result,
												arrName->getLabel());

	/* Restore the registers to the free list */
	restoreStore(offset);
	restoreStore(addrReg);
	restoreStore(result);
}

void GenericASTVisitor::visit(NAssignment *node) {
	int rval = node->getChildrenSize() - 1;
	int lval = rval-1;
	string storeAddr;
	/* Evaluate the rval and get the result from the front of the free regs */
	node->getChild(rval)->accept(this);
	string value = getNextReg();
	/* If it isn't a dummy assignment for procedure calls */
	if (node->getChildrenSize() > 1) {
		/*
		 * Get the declaration node, work out its nesting level and the
		 * difference in nesting level from the assignment so that we can
		 * follow access links to its place on the stack.
		 */
		Node* leftChildID = node->getChild(lval);
		Node* declarationNode = node->getTable()->lookup(leftChildID->getID());
		int nestingLevel = declarationNode->getLevel();
		int numJumps = node->getLevel() - nestingLevel;
		string label = declarationNode->getLabel();
		/*
		 * If the declaration was a reference then we need to get the value
		 * from the address.
		 */
		if (declarationNode->getType() == REFNUMBER ||
			declarationNode->getType() == REFCHAR) {
			/*
			 * Evaluate the left child, get its result reg from the free 
			 * list. 
			 */
			leftChildID->accept(this);
			storeAddr = getNextReg();
			/* Move the value into the the address pointed to by storeAddr */
			generator->printInstruction(AMOVE, "[" + storeAddr + "]", value);
			} else if (declarationNode->getType() == TSTRING) {
				/* 
				 * Set the label to point to the string lit declared in the
				 * data section.
				 */
				declarationNode->setLabel(node->getChild(rval)->getLabel());
			} else {
				/* Check if in local or global scope */
				if (nestingLevel == node->getLevel() || nestingLevel == 1) {
					/* If global just use data from label section. */
					if (nestingLevel == 1)
						label = "[" + label + "]";
					generator->printInstruction(AMOVE, label, value);
				} else {
					generator->printInstruction
							(APUSH, generator->getBasePointer());
					while(numJumps > 0) {
					/* Follow the access link back overwriting rbp */
						string bp = generator->getBasePointer();
						generator->printInstruction
								(AMOVE, bp, "[" + bp + "+16" + "]");
						numJumps--;
					}
					/* Mov the result into the correct location. */
					generator->printInstruction(AMOVE, label, value);
					/* pop rbp */
					generator->printInstruction
						(APOP, generator->getBasePointer());
					}
				}
			}
			restoreStore(storeAddr);
			restoreStore(value);
		}
void GenericASTVisitor::generateBinOpInstr(int op, string returnReg, string nxtReg) 
{
	/* Delegate printing to the specific code generator */
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
	/*
	 * Use the weight function to give more registers to the weightier
	 * of the two binary expressions. This optimisation is an example of
	 * the register allocation scheme seen in the lecture notes.
	 */
    string resultReg, nxtReg;
    if (node->getChild(0)->getWeight() > node->getChild(1)->getWeight()) {
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
    } else {
        node->getChild(1)->accept(this);
        nxtReg = getNextStore();
        node->getChild(0)->accept(this);
        resultReg = getNextReg();
    }
	
	/*
	 * If either are an array access, access the location pointed to by 
	 * result reg as they point to a space on the heap.
	 */
	if (node->getChild(0)->getNodeType() == ARRAYACCESS) {
		generator->printInstruction(AMOVE, resultReg, "[" + resultReg + "]");
	}
	if (node->getChild(1)->getNodeType() == ARRAYACCESS) {
		generator->printInstruction(AMOVE, nxtReg, "[" + nxtReg + "]");	
	}
		
	/* Delegate to the specific generator to print the relevant output. */
    generateBinOpInstr(node->getOp(), resultReg, nxtReg);   
    restoreStore(nxtReg);
    restoreStore(resultReg); 
}

void GenericASTVisitor::visit(NCharLit *node) {
    /*
	 * Give the generator a scratch register, followed by the char literal
	 * stored within the NCharLit node. 
	 */
	string reg = getNextReg();
	generator->printInstruction(AMOVE, reg, "'" + node->getID() + "'");
    restoreStore(reg);
}

void GenericASTVisitor::visit(NCodeBlock *node) {
	/*
	 * Push the base pointer three times to emulate the behaviour of 
	 * a function when pushing its return address and access link. This
	 * is a somewhat hacky solution, however it does work!
	 */
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(APUSH, generator->getBasePointer());

	generator->printInstruction(AMOVE, generator->getBasePointer(),
										generator->getStackPointer());
    
	for(int i = 0; i < node->getChildrenSize(); ++i) {
        node->getChild(i)->accept(this);
    }

	/* Return from the scope */
	generator->printInstruction(AMOVE, generator->getStackPointer(),
										generator->getBasePointer());
	
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
}

void GenericASTVisitor::visit(NConditional *node) {
 	/*
	 * If we are at the very top of a conditional, we must ask the
	 * label maker for a new end conditional node. This is so that
	 * if then else statements can always point towards the end of
	 * an arbitrarily long conditional.
	 */
    bool startConditional = this->labelMaker.needsNewEndCondLabel();
    string endLabel = this->labelMaker.getEndCondLabel();
    string elseLabel = this->labelMaker.getNewLabel();

    /* Visit the predicate to evaluate the expression */

    node->getChild(0)->accept(this);
    string resultReg = this->getNextReg();

    /* If condition is false, jump to next condition */

	generator->printInstruction(ACMP, resultReg, "1");
	generator->printInstruction(AJNE, elseLabel);    
    restoreStore(resultReg);

	/*
	 * We might need to generate a new end label for a nested condition, so
	 * we push the end label. Then the statement list is evaluated.
	 */
    this->labelMaker.pushEndCondLabel();
    node->getChild(1)->accept(this);
	/* Once accepting the child, we pop the end label. */
    this->labelMaker.popEndCondLabel();

    /* Return to end of conditional */
    generator->printInstruction(AJMP, endLabel);

    /* 
	 * In the case where the condition is false, print the else label and
	 * evaluate the statement list.
	 */
	generator->printLabel(elseLabel);
    node->getChild(2)->accept(this);

	/* If this is the top level conditional, print the end label at the 
	 * bottom.
	 */
    if (startConditional)  {
		generator->printLabel(endLabel);
        this->labelMaker.resetEndCondLabel();
    }
}

void GenericASTVisitor::visit(NEndIf *node) {} /* Blank node */

/* NDeclarationBlock is our 'entry' point - its not exclusve though, ie every 
 * function has one
 */
void GenericASTVisitor::visit(NDeclarationBlock *node) {
    /* 
	 * If we have a delcaration block just visit every child, set the offset
	 * to the access point to be 0.
	 */
    offset = 0;
    if (!node->isRoot()) {
        for (int i = 0; i < node->getChildrenSize(); i++) {
            node->getChild(i)->accept(this);
        }
    } else {
        for (int i = 0; i < node->getChildrenSize(); i++) {
            if (node->getChild(i)->getNodeType() != VARDEC) {
                node->getChild(i)->accept(this);
            }
        }
    }
}

void GenericASTVisitor::visit(NFunctionDeclaration *node) {
	/* 
	 *Store the label of the function in its identifier and add it to a
	 * queue of functions to prevent functions being declared inline.
	 */
    if (node->getLevel() != 1) {
        string label = "_" + node->getID() + this->labelMaker.getNewLabel();
        node->setLabel(label);
    }
    funcDecQueue.push(node);
}

void GenericASTVisitor::visit(NIdentifier *node) {
	/* 
	 * Find where the node was declared and find the difference between its
	 * nesting level and the nesting level of its caller.
	 */
    Node *declarationNode = node->getTable()->lookup(node->getID());
    int nestingLevel = declarationNode->getLevel();
    int numJumps = node->getLevel() - nestingLevel;
    string storage = getNextReg();
    /* Check if in local or global scope */
    if (nestingLevel == node->getLevel() || nestingLevel == 1) {
	if (nestingLevel == 1) {
		/* Use the label from the global scope */		
		generator->printInstruction(AMOVE, 
					storage, "[" + declarationNode->getLabel() + "]");
	} else {
		generator->printInstruction(AMOVE, storage,
					declarationNode->getLabel());
	}
    } else {
		/*
		 * Follow the access link to find the base pointer corresponding 
		 * to the declared variable's scope. 
		 */
        generator->printInstruction(APUSH, generator->getBasePointer());
        while(numJumps > 0) {
            /* follow the access link back overwriting rbp */
			string bp = generator->getBasePointer();
            generator->printInstruction(AMOVE, bp, "[" + bp + "+16" + "]");
            numJumps--;
        }
        /* Move from label into the correct location. */
			generator->printInstruction(AMOVE, storage,
				declarationNode->getLabel());
        /* Pop rbp */
			generator->printInstruction(APOP, generator->getBasePointer());
    }
    restoreStore(storage);
}

void GenericASTVisitor::visit(NInc *node) {
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
}

void GenericASTVisitor::visit(NDec *node) {
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
}
void GenericASTVisitor::visit(NInput *node) {
	/* Call the C Standard Library scanf function. If the variable is at
	 * global scope reference it directly. Pass the scanfstring and the
	 * address at where to store the input to the generator.
	 */
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
		/*
		 * Could do with implementing this for strings and characters, but
		 * time constraints have prevented this!
		 */
    }
}

void GenericASTVisitor::visit(NInteger *node) {
	/*
	 * Evaluate the node and store its direct value into the first free reg
	 * from the free list.
	 */
    string store = getNextReg();
	generator->printInstruction(AMOVE, store, convertInt(node->getValue()));
    restoreStore(store);
}

void GenericASTVisitor::visit(NLoop *node) {
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
}

void GenericASTVisitor::visit(NMethodCall *node) {
    /* Save registers currently in use */
    pushRegs();
    if (node->getChildrenSize() > 0) {
        node->getChild(0)->accept(this);
    } 

    Node* MethodDec = node->getTable()->lookup(node->getID());
	int numJumps = node->getLevel() - MethodDec->getLevel();
	string freeReg = getNextReg();

	/* Generate access link */
	generator->printInstruction(AMOVE, freeReg, generator->getBasePointer());
	while (numJumps > 0)
	{
		generator->printInstruction(AMOVE, freeReg, "[" + freeReg + "+16]");
		numJumps--;
	}

	/* Push the access link */
	generator->printInstruction(APUSH, freeReg); 
    /* call the method */
	generator->printInstruction(ACALL, "_" + MethodDec->getLabel());
	restoreStore(freeReg);
    /* Remove the access link from the stack */
	generator->printInstruction(AADD, generator->getStackPointer(), "8");    
    /* Remove paramaters from stack */
	if (node->getChildrenSize() > 0 ) {
    	    int numParams = node->getChild(0)->getChildrenSize();
		generator->printInstruction(AADD, generator->getStackPointer(),
							convertInt(numParams * 8));	
    }

    /* Restore registers we were using */
    popRegs();
	
	/* Move the result of the function into the next free reg in the list */
    string reg = getNextReg();
    generator->printInstruction(AMOVE, reg, generator->getReturnRegister());
    restoreStore(reg);
}

void GenericASTVisitor::visit(NNullToken *node) {} /* No operation */
/* 
 * If a case isn't implemented, it will fall through to an empty 
 * implementation. 
 */
void GenericASTVisitor::visit(Node *node) {} 

void GenericASTVisitor::visit(NParamBlock *node) {
    unsigned int numChildren =  node->getChildrenSize();
    string label;
    std::deque<string>::iterator it = allRegs.begin();
    /*
	 * Push params onto the stack in reverse order so that the caller has 
	 * access to the first paramater in the first position above its access
	 * link, and so forth.
	 */   
    for (int i = numChildren - 1; i >= 0; i--) {
        node->getChild(i)->accept(this);
		string reg = getNextReg();
		if (node->getChild(i)->getNodeType() == ARRAYACCESS) {
			generator->printInstruction(AMOVE, reg, "[" + reg + "]");	
		}
        generator->printInstruction(APUSH, reg);  
    }
}

/*
 * Labels the position of parameters, i.e. allocates them stack space or
 * global space
 */
void GenericASTVisitor::visit(NParamDeclarationBlock *node) {
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
    /* Put params onto the stack */
    for (unsigned int i = 0; i < numChildren; i++) {
		label = "[" + generator->getBasePointer() + "+";
        label = label + boost::lexical_cast<string>(offset) + "]";
        node->getChild(i)->setLabel(label);
        offset+=8;
    }
}

/* 
 * Generic print function that checks the type of the node that you wish
 * to print and delegates the correct parameters to the specific target 
 * language code generator as necessary.
 */
void GenericASTVisitor::visit(NPrint *node) {
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
	if (decType == REFCHAR || decType == REFNUMBER)	{
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
}

void GenericASTVisitor::visit(NReturn *node) {
    /* Move the return value into the result register and call return. */
	Node *retVal = node->getChild(0);
    retVal->accept(this);
    string reg = getNextReg();
	generator->printInstruction(AMOVE, generator->getReturnRegister(), reg);
    restoreStore(reg);
    ret();
}

void GenericASTVisitor::visit(NStatementList *node) {
    for (int i = 0; i < node->getChildrenSize(); i++)
    {
        node->getChild(i)->accept(this);
    }
}

void GenericASTVisitor::visit(NStringLit *node) {
	/* 
	 * Store the literal in the data section and point the node's label 
	 * to the label.
 	 */
    string label = this->labelMaker.getNewLabel();
	generator->printStringLitData(label, node->getID()); 
    node->setLabel(label);
    string reg = getNextReg();
	generator->printInstruction(AMOVE, reg, label);
    restoreStore(reg);
}

void GenericASTVisitor::visit(NUnaryOp *node) {
    node->getChild(0)->accept(this);
    string resultReg = getNextReg();
    switch(node->getOp()) {
        case DASH:
			generator->printInstruction(ANEG, resultReg);
            break;
        case LNOT:
        case NOT:
			generator->printInstruction(ANOT, resultReg);
            break;
        case PLUS:
			/* Empty implementation, PLUS does nothing. */
            break;
    }
    restoreStore(resultReg);
}
/* Variables are allocated on the stack, and are given
 * an address as [rbp +/- offset]
 */
void GenericASTVisitor::visit(NVariableDeclaration *node) {
    int type = node->getType();
	stringstream numElementsStr;
	Node *lengthNode;
	string label;
    int numElements; 
    stringstream convert;
    switch(type) {
		/* Reserve 8 bytes for a character or number */
        case TNUMBER:
        case TCHAR:
            offset += 8;
			generator->printInstruction(ASUB, 
							generator->getStackPointer(), "8"); 
			convert << "[" << generator->getBasePointer() << "-" << offset << "]";
	    if (node->getLevel() != 1)
	        node->setLabel(convert.str());           
		break;
		/* For a reference reserve a number of elements as neccessary */
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
			/* Reserve numelements * 8 bytes worth of space on the stack */
            /* Set the label to be a pointer to the offest of the 1st element */
            convert << "[" << label << "]";
            /* Set the offest to take into account the entire array */
			if (lengthNode->getNodeType() == INTEGER) {
				offset+= numElements;
			}
            node->setLabel(convert.str());
        break;
        case TSTRING:
        break;
    }
}

/* Print the label of the subroutine, push the base pointer. */
void GenericASTVisitor::createSubroutine(string name) {
	generator->printLabel("_" + name);
	generator->printInstruction(APUSH, generator->getBasePointer());
	generator->printInstruction(AMOVE, generator->getBasePointer(),
								generator->getStackPointer());    
}

/* Similar to before, restore the base pointer and return from function. */
void GenericASTVisitor::ret() {
	generator->printInstruction(AMOVE, generator->getStackPointer(), 
										generator->getBasePointer());
	generator->printInstruction(APOP, generator->getBasePointer());
	generator->printInstruction(ARET);
}

/* Return the assembly generated from the code generator */
string GenericASTVisitor::getAssembly() {
	return generator->getAssembly();
}

/* 
 * Pop functions from the queue to generate code for, also print the hatta
 * label as the entry point.
 */
void GenericASTVisitor::generateFunctionDefinitions()
{
	generator->printInstruction(ACALL, "_hattalabel1");
	generator->printReturnFromProgram();
    
    while (!funcDecQueue.empty()) {
        unfoldedFunctionVisitor(funcDecQueue.front());
        funcDecQueue.pop();
    }
}

/* Unfolds functions so that nested functions are not printed inline but are
 * instead printed sequentially in a file. To print functions inline would
 * produce jump instructions preceeding the function to ensure code 
 * does not fall through, this is thus a nicer approach!
 */
void GenericASTVisitor::unfoldedFunctionVisitor(NFunctionDeclaration* node) {
    /* Sets up our function label and base pointer.*/
    createSubroutine(node->getLabel());
    int numChildren = node->getChildrenSize();
    
    if (numChildren == 2) {
        /* Store the parameters into locations. */
        NParamDeclarationBlock* params = 
			(NParamDeclarationBlock*) node->getChild(numChildren - 1);
        params->accept(this);
    }
    NCodeBlock* codeblock = (NCodeBlock*)node->getChild(0);
    NStatementList* statlist = (NStatementList*)codeblock->getChild(0);
    if (codeblock->getChildrenSize() == 2) {
        /* We have local variables*/
        NDeclarationBlock* decblock = (NDeclarationBlock*)codeblock->getChild(0);
        statlist = (NStatementList*)codeblock->getChild(1);
        /* Reserve space on stack from them */
        decblock->accept(this);
    } 
    statlist->accept(this);    
    freeRegs = allRegs;
	ret();
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

    /* Get all global variables and string literals used in ext...*/
    boost::shared_ptr<SymbolTable> t = root->getTable();
    table_t::iterator it;

    string labelSuffix = this->labelMaker.getNewLabel();

    /* Reserve space for global variables." */
    for(it = t->start(); it != t->end(); it++) {
        string label = it->first + labelSuffix;
		generator->printData(label, convertInt(it->second->getSize()));
        it->second->setLabel(label);
    }

	/* Print any extern functions we need */
	generator->printExtern(); 
}

/* 
 * Returns the next store, this could be a register or a location on the
 * stack.
 */
string GenericASTVisitor::getNextStore() {
    string res;
    if (freeRegs.size() == 1) {
		res = "[" + generator->getStackPointer() + "]";
    } else {
        res = getNextReg();
    }
    return res;
}

/*
 * Returns the next free scratch register to use. If we have exhausted all
 * registers, we use the last one as an accumulator. Convention is that we
 * always restore the result of an evaluation into the first position in
 * the free regs list, so after visiting a child, calling getNextReg() will
 * get the resultant evaluation.
 */
string GenericASTVisitor::getNextReg() {
    string res;
    if (freeRegs.size() == 1) {
        res = freeRegs.front();
    } else {
        res = freeRegs.front();
        freeRegs.pop_front();
    }
    return res;
}

void GenericASTVisitor::restoreStore(string store) {
    if (find(allRegs.begin(), allRegs.end(), store) != allRegs.end()) {
        freeRegs.push_front(store);
    }

}

/* Pushes registers that are 'in use' */
void GenericASTVisitor::pushRegs() {
    std::deque<string>::iterator it;
    for (it = allRegs.begin(); it != allRegs.end(); it++ ) {
        if (find(freeRegs.begin(),freeRegs.end(),(*it)) == freeRegs.end())
            generator->printInstruction(APUSH, (*it));
    }
    callStackRegs.push_front(freeRegs);
    freeRegs = allRegs;
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
   stringstream ss;
   ss << number;
   return ss.str();
}
