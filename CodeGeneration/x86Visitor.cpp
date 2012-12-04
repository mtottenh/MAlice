#include "x86Visitor.hpp"
#include <boost/shared_ptr.hpp>
#include "../Node/NodeIncludes.hpp"

x86Visitor::x86Visitor() : labelMaker() {}


void x86Visitor::visit(NArrayAccess *node) {

}

void x86Visitor::visit(NAssignment *node) {

}
/* TODO: still need to implement a register saving mechanism 
 * maybe paramerterise visit with a list of reg's it can use
 * like the tutorial.
 */
void x86Visitor::visit(NBinOp *node) {
		node->getChild(0)->accept(this);
        node->getChild(1)->accept(this);

				
}

void x86Visitor::visit(NCharLit *node) {

}

void x86Visitor::visit(NCodeBlock *node) {

}

void x86Visitor::visit(NConditional *node) {

}

/* NDeclarationBlock is our 'entry' point */
void x86Visitor::visit(NDeclarationBlock *node) {
    cerr << "Declaration Block" << endl;
}

void x86Visitor::visit(NEndIf *node) {

}

void x86Visitor::visit(NFunctionDeclaration *node) {
    /* Sets up our function label and base pointer*/
    createSubroutine(node->getID());
    int numChildren = node->getChildrenSize();
    
    if (numChildren == 2) {
        /* we have paramaters */
        /* TODO get dat stuff from the stack and put into regs son*/
       NParamDeclarationBlock* params = (NParamDeclarationBlock*) node->getChild(numChildren - 1);
      
    }
    NCodeBlock* codeblock = (NCodeBlock*)node->getChild(0);
    NStatementList* statlist = (NStatementList*)codeblock->getChild(0);
    if (codeblock->getChildrenSize() == 2) {
        /* we have local variables*/
        NDeclarationBlock* decblock = (NDeclarationBlock*)node->getChild(0);
        statlist = (NStatementList*)codeblock->getChild(1);
        /* reserve space on stack from them */
        /*TODO should probably get some way of total size of local vars*/
        /* rather than just assuming they are all 32bits */
        text << "\tsub rsp," << decblock->getChildrenSize() * 4 <<"\n";
    } 
    statlist->accept(this);    

}

void x86Visitor::visit(NIdentifier *node) {

}

void x86Visitor::visit(NInc *node) {

}

void x86Visitor::visit(NDec *node) {

}

void x86Visitor::visit(NInput *node) {

}

void x86Visitor::visit(NInteger *node) {

}

void x86Visitor::visit(NLoop *node) {

}

void x86Visitor::visit(NMethodCall *node) {

}

void x86Visitor::visit(NNullToken *node) {
	cout << "Visiting a null token" << endl;
}

void x86Visitor::visit(Node *node) {

}

void x86Visitor::visit(NParamBlock *node) {
	/* Here we will need to push the children onto the stack
	   in the reverse order */
}

void x86Visitor::visit(NParamDeclarationBlock *node) {
	/* I think this is only used in the semantic analysis,
	   past that assembly code doesn't know about the params
	   passed to a function. We might need to create symbolic
	   references between their position on the stack in order
	   to utilise the stack values */
}

void x86Visitor::visit(NPrint *node) {
    /* save any reigsters we are going to use 
    text << "push rax\n"; 
    text << "push rcx\n";    
    text << "push rbx\n";    */
    /* evaluate what we want to print */
    /* the contract for this is that it has to leave
     * the thing to print in rax??
     */   
    node->getChild(0)->accept(this);
    /* We can use the macro defed in system.inc */
    text << "\tsys.write\n";
}

void x86Visitor::visit(NReturn *node) {
    cerr << "Return Node" << endl;
}

void x86Visitor::visit(NStatementList *node) {
    for (int i = 0; i < node->getChildrenSize(); i++)
	{
		node->getChild(i)->accept(this);
	}
	cerr << "Statement List" << endl;
}

void x86Visitor::visit(NStringLit *node) {
    /* Add strings to the .data section 
     * TODO: We still need to find a way
     * of labeling stringlits and their sizes
     *
     */
    /* Really bad complexity, basically create a copy,find the data section
     * and insert our new string into the data section TROLOLOL
     */
    data << "\nAUTOGEN:\tdb\t";
    data << node->getID(); 
    data << "\nAUTOGENS:\tequ $-AUTOGEN:\n";


}

void x86Visitor::visit(NUnaryOp *node) {
    cerr << "UnaryOp" << endl;
}

void x86Visitor::visit(NVariableDeclaration *node) {
    cerr << "Variable Declaration" << endl;
    ///* if we are in the global scope*/
  //  node->getType();
//    text << 
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

void x86Visitor::saveCalleeReg() {
    text << "push rbx\n";
    text << "push rdi\n";
    text << "push rsi\n";
}    

void x86Visitor::restoreCalleeReg() {
    /** Return from subroutine**/
    text << "pop rsi\n";
    text << "pop rdi\n";
    text << "pop rbx\n";
}
void x86Visitor::deallocVar() {

}
void x86Visitor::ret() {
    text << "mov esp,ebp\n";
    text << "pop ebp\n";   
    text << "ret\n";
}

string x86Visitor::getAssembly() {
	data << text.str();
    return data.str();
}

void x86Visitor::init(Node* root) {
    /* initial call to set up our .data section*/
    data << "%include\t\'system.inc\'\n\n";
    data << "section .data\n";
    /* get all global variables and string literals used in text...*/
    boost::shared_ptr<SymbolTable> t = root->getTable();
    table_t::iterator it;
    /* global vars, reserve space for em init" */
    for(it = t->start(); it != t->end(); it++) {
        data << "common\t" << it->first << "  " 
                << it->second->getSize() << endl;
    }
 
    text << "section .text\n";
    text << "global _start\n";
    text << "_start:\n";
}
