#ifndef __TYPEMAP_H__
#define __TYPEMAP_H__

#include <map>
#include <string>

using namespace std;

extern map<int, string> typemap;
int typemap_add(int, string);
string typemap_get(int);

#endif
