#include "LabelMaker.hpp"

LabelMaker::LabelMaker() : currentLabelNumber(1) {} 

string LabelMaker::getNewLabel() 
{
	return string("label%f", currentLabelNumber++);
}

string LabelMaker::getEndCondLabel()
{
	if (!this->endLabel)
	{
		this->endLabel = new string(this->getNewLabel());
	}
	return *(this->endLabel);
}

void LabelMaker::pushEndCondLabel()
{
	this->endCondLabelStack.push(this->endLabel);
	this->resetEndCondLabel();
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
