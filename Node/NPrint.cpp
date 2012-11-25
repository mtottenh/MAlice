#include "NPrint.hpp"
#include "TypeDefs.hpp"

NPrint::NPrint() {
	name = "Print";
}

NPrint::NPrint(const char* msg) {
	message = strdup(msg); 
	name = "Print";
}

NPrint::NPrint(Node* child) { 
	name = "print";
	children.push_back(child); 
}

int NPrint::check() {
	/* If we have no child nodes, we have the const char* case. */
	if(children.size() == 0) {
		return 1;
	}

	/* Otherwise, check that the node has a type that can be printed. */
	int childType = children[0]->getType(); 

	if(childType == TNUMBER || childType == REFNUMBER
			|| childType == TCHAR || childType == REFCHAR
			|| childType == TSTRING || childType == REFSTRING) {
		return 1;
	}
	
	else {
		error_type_mismatch(SAID, childType, "number/letter/sentence");
		return 0;
	}
}
