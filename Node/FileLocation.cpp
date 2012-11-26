#include "FileLocation.hpp"

using namespace std;

FileLocation::FileLocation(FileLocation* loc)
{
	this->startLine = loc->startLine;
	this->endLine = loc->endLine;
	this->startColumn = loc->startColumn;
	this->endColumn = loc->endColumn;
}
