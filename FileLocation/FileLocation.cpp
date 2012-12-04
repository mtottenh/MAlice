#include "FileLocation.hpp"

FileLocation::FileLocation(FileLocation* loc)
{
	this->startLine = loc->startLine;
	this->endLine = loc->endLine;
	this->startColumn = loc->startColumn;
	this->endColumn = loc->endColumn;
}

FileLocation::FileLocation(const FileLocation &loc)
{
	this->startLine = loc.startLine;
	this->endLine = loc.endLine;
	this->startColumn = loc.startColumn;
	this->endColumn = loc.endColumn;
}
