#include "TypeMap.hpp"
#include <boost/lexical_cast.hpp>

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
		return boost::lexical_cast<string>(typeval);
	}
	else {
		return it->second;
	}
}
