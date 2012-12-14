#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"
NFunctionDeclaration::NFunctionDeclaration() { 
    name = "FuncDec"; 
}                            

/* Declaring a procedure with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block) {
    this->type = VOID;
    children.push_back(block);
    name = ID->getID(); 
    nodeType = PROCEDURE;
    delete(ID);
}                                                                        
/* Delcaring Function with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block, 
                        int type) {
    this->type = type;
    name = ID->getID();
    children.push_back(block);
    nodeType = FUNC;
    delete(ID);
}

/* declaring a function with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params, 
                         Node *block, int type) {
    this->type = type;
    name = ID->getID();
    delete(ID);
    children.push_back(block);
    children.push_back(params);
    nodeType = FUNC;
}

/* Declaring a procedure with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, 
                        Node *params,Node *block) {
    this->type = VOID;
    name = ID->getID();
    children.push_back(block);
    children.push_back(params);
    nodeType = PROCEDURE;
    delete ID;
}

/* TODO remove print and gettype
 * these should be inherited from Node anyway
 */
int NFunctionDeclaration::print() const {
    cout << name ;                                                   
    return 1;                                                        
}
int NFunctionDeclaration::getType() {
    return type;
}


int NFunctionDeclaration::check() {
    int isValid = 1;
    Node *statlist; 
    unsigned int numNReturn;
    /*initialize a list of pointers to NReturn nodes, we will use this later*/
    std::deque<Node *> returnList;
    returnList.clear();

    /* The codeblock is always the 1st child */
    Node* codeBlock = children[0];
    int numChildren = codeBlock->getChildrenSize();

    /* Empty functions are invalid*/
    if (numChildren == 0 && nodeType == FUNC) {
        isValid = 0;
    } 
    /* The statement list is always the last child of an NCodeBlock */   
    statlist = codeBlock->getChild(numChildren - 1);

    /* Generate a list of NReturn Nodes */
    if (statlist != NULL ) {
        returnList = returnNodeList(statlist); 
    }

    numNReturn = returnList.size();

    /* Loop through the NReturns and check that the types match*/
    if (nodeType == FUNC) {
        for (unsigned int i = 0; i < numNReturn; i++) {
    
            if( !(returnList[i]->getType() == type)) {
                printErrorHeader("function declaration");
                error_return_type(name, type, returnList[i]->getType());
                isValid = 0;
            }
    
       }
            /* Also check that there are some NReturn nodes*/
            isValid &= !(numNReturn == 0);
    } else {
            /* Procedures are only valid if they contian no NReturn nodes*/
             isValid &= (numNReturn == 0);
    }
    
    /* Print Errors if we encountered any */
    if (!isValid) {
        if (nodeType == FUNC) {
            printErrorHeader("function declaration");
            error_no_return(name); 
        } else  {
            printErrorHeader("procedure declaration");
            error_proc_return(name);
        }
    }
    /* check the children are valid */  
    isValid &= Node::check();

    return isValid;
}

std::deque<Node *>  NFunctionDeclaration::returnNodeList(Node* statlist) {
    std::deque<Node *> returnList, childList, conditionalQ;
    unsigned int size = statlist->getChildrenSize();
    unsigned int numChild;
    Node *child, *nodePtr;
    returnList.clear();

    for (unsigned int i = 0; i < size; i++ ) {
        child = statlist->getChild(i);
        switch(child->getNodeType()) {
            case RETURN:
                returnList.push_back(child);
                break;

            case CONDITIONAL:
                    conditionalQ.push_back(child);
                    while(!conditionalQ.empty()) {
                        nodePtr = conditionalQ.front();
                        conditionalQ.pop_front();
                        if (nodePtr->getNodeType() == CONDITIONAL) {
                            /* Left branch always has nodetype STATMENTLIST*/
                            childList = returnNodeList(nodePtr->getChild(1));
                            conditionalQ.push_back(nodePtr->getChild(2));
                        } else {
                            childList = returnNodeList(nodePtr);
                        }
                         numChild  = childList.size();
                         for( unsigned int j = 0; j < numChild; j++) 
                                returnList.push_back(childList[j]);
                    }
                break;

            case LOOP:
            case CODEBLOCK:
                if (child->getChildrenSize() > 0) {
                    nodePtr = child->getChildren().back();
                }
                childList = returnNodeList(nodePtr);
                numChild =  childList.size();
                for( unsigned int j = 0; j < numChild; j++) {
                    returnList.push_back(childList[j]);
                }
                break;
        }
    
    }
    return returnList;
}

void NFunctionDeclaration::accept(ASTVisitor* v) {
    v->visit(this);
}

#endif
