#ifndef _LABELMAKER_H_
#define _LABELMAKER_H_

#include <string>
#include <stack>

using namespace std;

class LabelMaker
{
private:
	int currentLabelNumber;
	string* endLabel;
	stack<string*> endCondLabelStack;
public:
	LabelMaker();
	string getNewLabel();
	string getEndCondLabel();
	bool needsNewEndCondLabel();
	void resetEndCondLabel();
	void pushEndCondLabel();
	void popEndCondLabel();
};

#endif
