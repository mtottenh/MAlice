#ifndef _CODEGENERATOR_H_
#define _CODEGENERATOR_H_

#include <string>
#include <sstream>
#include <map>
#include <deque>

typedef enum Instr
{
	AMOVE, ASUB, AADD, AIMUL, ADIV, AOR, AAND, AXOR, ACMP, APUSH, APOP, 
	AMOVEXTEND,	AJMP, AJNE, AJE, AINC, ADEC, ACALL, ANEG, ANOT, ARET, AMOD,
	SETL, SETLE, SETG, SETGE, SETE, SETNE, ALEA, AMALLOC, ALDR  
} Instr;

using namespace std;

class CodeGenerator
{
protected:
	stringstream text;
	stringstream data;
	map<Instr, string> instrTable;
	void printTab();
	int wordSize;
public:
	CodeGenerator();
	void printLabel(string);
	string getAssembly();
	//Hook methods.
	virtual void printInstruction(Instr);
	virtual void printInstruction(Instr, string);
	virtual void printInstruction(Instr, string, string);
	virtual deque<string> getAllGeneralRegs() = 0 ;
	virtual void printData(string, string) = 0 ;
	virtual void printStringLitData(string, string) = 0 ;
	virtual void printInitial() = 0 ;
	virtual void printExtern() = 0 ;
	virtual string getBasePointer() = 0 ;
	virtual string getStackPointer() = 0 ;
	virtual string getReturnRegister() = 0 ;
	virtual void generatePrintInstruction(string, string, bool, int) = 0 ;
	virtual void comparePredicate(Instr, string, string) = 0 ;
	virtual void printReturnFromProgram() = 0 ;
	virtual void printHeapAllocationInstruction(string, string) = 0 ;
	virtual void generateAccessInstruction(string, string, string, string) = 0 ;
	virtual void generateInputFunction(string, string) = 0 ;
	virtual string generateMemoryOffset(string, int) = 0;
};

#endif
