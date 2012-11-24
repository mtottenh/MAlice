#ifndef _NIDENTIFIER_HPP_
#define _NIDENTIFIER_HPP_

#include "Node.hpp"
#include <string>

class NIdentifier : public Node {
        public:
        	NIdentifier();
		string getID();
		NIdentifier(string);
	      	NIdentifier(char*);                                
		int check(); 
		int getType() const;
};  

#endif
