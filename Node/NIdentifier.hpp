#ifndef _NIDENTIFIER_HPP_
#define _NIDENTIFIER_HPP_

#include "Node.hpp"

class NIdentifier : public Node {                                         
        public:                                                                  
        NIdentifier() { name = "ID"; }                                           
//      NIdentifier(char *a) { name = strdup(a);                                 
};  

#endif
