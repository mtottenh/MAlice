#ifndef _NFUNCTIONDECLARATION_HPP_
#define _NFUNCTIONDECLARATION_HPP_

#include "NDeclarationBlock.hpp"

/*class NExpressio nStatement*/                                                  
                                                                                 
class NFunctionDeclaration : public NDeclarationBlock {                          
                                                                                 
public:                                                                          
        NFunctionDeclaration () { name = "FuncDec"; }                            
        NFunctionDeclaration (NCodeBlock *block) {                               
                name = "FuncDec";                                                
                children.push_back(block);                                       
        }                                                                        
        int print() const {                                                      
                cout << name ;                                                   
                return 1;                                                        
        }                                                                        
};    

#endif
