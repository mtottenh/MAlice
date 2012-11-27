#ifndef _FILELOCATION_H_
#define _FILELOCATION_H_

class FileLocation
{
public:
	FileLocation() {}
	FileLocation(FileLocation*);
	FileLocation(const FileLocation&);
	int startLine;
	int endLine;
	int startColumn;
	int endColumn;
};

#endif
