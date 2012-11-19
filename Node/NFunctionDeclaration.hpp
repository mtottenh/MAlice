#ifndef _NFUNCTIONDECLARATION_HPP_
#define _NFUNCTIONDECLARATION_HPP_

#include "Node.hpp"

/*class NExpressio nStatement*/                                                  
                                                                                 
class NFunctionDeclaration : public Node {                          
                                                                                 
public:                                                                          
        NFunctionDeclaration () { name = "FuncDec"; }                            
        NFunctionDeclaration (Node *block) {                               
                name = "FuncDec";                                                
                children.push_back(block);                                       
        }                                                                        
        int print() const {                                                      
                cout << name ;                                                   
                return 1;                                                        
        }                                                                        
};    

#endif
