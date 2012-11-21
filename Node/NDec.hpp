#ifndef _NDEC_HPP
#define _NDEC_HPP

#include "Node.hpp"
#include "NIdentifier.hpp"

class NDec : public NIdentifier {
public:
	NDec(Node *child); 
}; 

#endif
