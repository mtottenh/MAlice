#ifndef _ARMCODEGENERATOR_H_
#define _ARMCODEGENERATOR_H_

#include "CodeGenerator.hpp"

class ARMCodeGenerator : public CodeGenerator
{
private:
	void evalMove(string, string);
	int isReg(string);
	deque<string> allRegs;
public:
using CodeGenerator::printInstruction;
	ARMCodeGenerator();
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
	virtual void printInstruction(Instr, string);
	virtual void printInstruction(Instr, string, string);
	virtual void comparePredicate(Instr, string, string);
	void printHeapAllocationInstruction(string, string);
	virtual void generateAccessInstruction(string, string, string, string);
	virtual void generateInputFunction(string, string);
	virtual string generateMemoryOffset(string, int);
};

#endif
