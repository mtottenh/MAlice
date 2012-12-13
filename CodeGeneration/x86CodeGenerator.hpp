#ifndef _X86CODEGENERATOR_H_
#define _X86CODEGENERATOR_H_

#include "CodeGenerator.hpp"

class x86CodeGenerator : public CodeGenerator
{
public:
using CodeGenerator::printInstruction;
	x86CodeGenerator();
	deque<string> getAllGeneralRegs();
	virtual string getBasePointer();
	virtual string getStackPointer();
	virtual string getReturnRegister();
	virtual void printData(string, string);
	virtual void printStringLitData(string, string);
	virtual void printInitial();
	virtual void printExtern();
	virtual void generatePrintInstruction(string, string, bool, int);
	virtual void printReturnFromProgram();
	virtual void printInstruction(Instr, string, string);
	virtual void comparePredicate(Instr, string, string);
	void printHeapAllocationInstruction(string, string);
	virtual void generateAccessInstruction(string, string, string, string);
	virtual void generateInputFunction(string, string);
};

#endif
