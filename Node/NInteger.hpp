#ifndef _NINTEGER_HPP_
#define _NINTEGER_HPP_

#include "Node.hpp"
#include <boost/lexical_cast.hpp>

class NInteger : public Node {
private:
	int value;
public:
        NInteger();
	NInteger(int);
};

#endif
