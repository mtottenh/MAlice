#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"
#include "TypeDefs.hpp"

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
    

    /* The codeblock is always the 1st child */
    Node* codeBlock = children[0];
    int numStatements = codeBlock->getChildrenSize();

    /* Empty functions are invalid*/
    if (numStatements == 0 && nodeType == FUNC) {
        isValid = 0;
    } else {
        /* The statement list is always the last child of an NCodeBlock */   
        statlist = codeBlock->getChild(numStatements - 1);

        /* Generate a list of NReturn Nodes */
        std::deque<Node *> returnList = returnNodeList(statlist); 
        unsigned int size = returnList.size();

        /* Loop through the NReturns and check that the types match*/
        if (nodeType == FUNC) {
            for (unsigned int i = 0; i < size; i++) {
                if( !(returnList[i]->getType() == type)) {
                    printErrorHeader("function declaration");
                    error_return_type(name, type, returnList[i]->getType());
                    isValid = 0;
                }
            }
            /* Also check that there are some NReturn nodes*/
            isValid &= !(size == 0);
        } else {
            /* Procedures are only valid if they contian no NReturn nodes*/
            isValid &= (size == 0);
        }
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
    std::deque<Node *> returnList, leftList, rightList, conditionalQ;
    if (statlist == NULL) {
        returnList.clear();
        return returnList;

    }
    unsigned int size = statlist->getChildrenSize();

    unsigned int sizeRight, sizeLeft;
    Node *left, *right;

    if ( size == 0) {
        returnList.clear();
        return returnList;
    }

    for (unsigned int i = 0; i < size; i++ ) {
        switch(statlist->getChild(i)->getNodeType()) {
            case RETURN:
                returnList.push_back(statlist->getChild(i));
                break;
            case CONDITIONAL:
                /* Left branch always has nodetype STATMENTLIST*/
                left = statlist->getChild(i)->getChild(1);
                leftList = returnNodeList(left);
                sizeLeft  = leftList.size();
                for( unsigned int j = 0; j < sizeLeft; j++) {
                    returnList.push_back(leftList[j]);
                }

                /* right could have nodetype CONDITIONAL!*/
                
                right = statlist->getChild(i)->getChild(2);
                if (right->getNodeType() == STATLIST) {

                    rightList = returnNodeList(right);
                    sizeRight = rightList.size();
                    for( unsigned int j = 0; j < sizeLeft; j++) {
                        returnList.push_back(rightList[j]);
                    }
                } 
                if (right->getNodeType() == CONDITIONAL) {
                    conditionalQ.push_back(right);
                    while(!conditionalQ.empty()) {
                        Node* temp = conditionalQ.front();
                        conditionalQ.pop_front();
                        if (temp->getNodeType() == CONDITIONAL) {
                            /* Left branch always has nodetype STATMENTLIST*/
                            left = temp->getChild(1);
                            leftList = returnNodeList(left);
                            sizeLeft  = leftList.size();
                            for( unsigned int j = 0; j < sizeLeft; j++) {
                                returnList.push_back(leftList[j]);
                            }
                            conditionalQ.push_back(temp->getChild(2));
                        }
                    }
                }
                break;
            case LOOP:
            case CODEBLOCK:
                if (statlist->getChild(i)->getChildren().size() > 0) {
                    left = statlist->getChild(i)->getChildren().back();
                }
                leftList = returnNodeList(left);
                sizeLeft =  leftList.size();
                for( unsigned int j = 0; j < sizeLeft; j++) {
                    returnList.push_back(leftList[j]);
                }
                break;
        }
    
    }
    return returnList;
}
/* TODO  finish check function */

#endif
