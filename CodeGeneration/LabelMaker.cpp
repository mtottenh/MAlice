#include "LabelMaker.hpp"
#include <iostream>
#include <sstream>

using namespace std;

LabelMaker::LabelMaker() : currentLabelNumber(1), endLabel(NULL) {} 

string LabelMaker::getNewLabel() 
{
	stringstream s;
	s << "label" << currentLabelNumber++;
	return s.str();
}

string LabelMaker::getEndCondLabel()
{
	if (this->needsNewEndCondLabel())
	{
		this->endLabel = new string(this->getNewLabel());
	}
	return *(this->endLabel);
}

bool LabelMaker::needsNewEndCondLabel()
{
	return !(this->endLabel);
}

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

void LabelMaker::resetEndCondLabel()
{
	delete this->endLabel;
	this->endLabel = NULL;
}
