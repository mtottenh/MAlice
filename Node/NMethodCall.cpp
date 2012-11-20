#ifndef ___NMETHODCALL__
#define ___NMETHODCALL__
#include "NMethodCall.hpp"

NMethodCall::NMethodCall(NIdentifier* id, Node* params) {
	name = id->getID();
	children.push_back(params);
	delete(id);
}
NMethodCall::NMethodCall(NIdentifier* id) {
	name = id->getID();
	delete(id);
}
#endif
