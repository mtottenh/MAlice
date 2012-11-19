#ifndef _NVARIABLEDECLARATION_HPP_
#define _NVARIABLEDECLARATION_HPP_

#include "NStatement.hpp"

class NVariableDeclaration : public NDeclarationBlock {                                 
public:                                                                          
        NVariableDeclaration() { name = "Declaration"; }                         
}; 

#endif
