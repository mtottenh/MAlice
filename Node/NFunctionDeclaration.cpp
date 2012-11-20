#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"

NFunctionDeclaration::NFunctionDeclaration() { 
	name = "FuncDec"; 
}                            

NFunctionDeclaration::NFunctionDeclaration (Node *block) {
	name = "FuncDec";                                                
        children.push_back(block);                                       
}                                                                        

int print() const {                                                      
	cout << name ;                                                   
	return 1;                                                        
}

#endif
