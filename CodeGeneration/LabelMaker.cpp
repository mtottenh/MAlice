#include "LabelMaker.hpp"
#include <iostream>
#include <sstream>

using namespace std;

LabelMaker::LabelMaker() : currentLabelNumber(1), endLabel(NULL) {} 

/* Prints a generic label */
string LabelMaker::getNewLabel() 
{
	stringstream s;
	s << "label" << currentLabelNumber++;
	return s.str();
}

/* Prints a label for an end conditional */
string LabelMaker::getEndCondLabel()
{
	if (this->needsNewEndCondLabel())
	{
		this->endLabel = new string(this->getNewLabel());
	}
	return *(this->endLabel);
}

/* Ensures that next time an end cond label is gathered, it is created new */
bool LabelMaker::needsNewEndCondLabel()
{
	return !(this->endLabel);
}

/* Pushes the end condition label onto a stack so it is not overwritten. */
void LabelMaker::pushEndCondLabel()
{
	this->endCondLabelStack.push(this->endLabel);
	this->endLabel = NULL;
}

void LabelMaker::popEndCondLabel()
{
	this->resetEndCondLabel();
	this->endLabel = this->endCondLabelStack.top();
	this->endCondLabelStack.pop();
}

/* Deletes the label and sets its pointer to NULL. */
void LabelMaker::resetEndCondLabel()
{
	delete this->endLabel;
	this->endLabel = NULL;
}
