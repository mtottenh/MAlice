#ifndef _NIDENTIFIER_HPP_
#define _NIDENTIFIER_HPP_

#include "NExpression.hpp"

class NIdentifier : public NExpression {                                         
        public:                                                                  
        NIdentifier() { name = "ID"; }                                           
//      NIdentifier(char *a) { name = strdup(a);                                 
};  

#endif
