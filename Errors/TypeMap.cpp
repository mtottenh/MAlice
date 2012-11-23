#include "TypeMap.hpp"

map<int, string> typemap;

int typemap_add(int typeval, string name) {
	typemap[typeval] = name;
	return 1;
}

string typemap_get(int typeval) {
	map<int, string>::iterator it = typemap.find(typeval);
	if(it == typemap.end()) {
		cerr << "ERROR: Invalid lookup in TypeMap. A type int value "
			<< "-> string representation is missing!" << endl;
		return "unknown";
	}
	else {
		return it->second;
	}
}
