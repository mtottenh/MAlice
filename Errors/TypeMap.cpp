#include "TypeMap.hpp"

map<int, string> typemap;

int typemap_add(int typeval, string name) {
	typemap[typeval] = name;
	return 1;
}

string typemap_get(int typeval) {
	map<int, string>::iterator it = typemap.find(typeval);
	if(it == typemap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}
