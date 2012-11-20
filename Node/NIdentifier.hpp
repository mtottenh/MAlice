#ifndef _NIDENTIFIER_HPP_
#define _NIDENTIFIER_HPP_

#include "Node.hpp"

class NIdentifier : public Node {                                         
        public:                                                                  
        	NIdentifier();                                           
		string getID();
//      	NIdentifier(char*);                                 
};  

#endif
