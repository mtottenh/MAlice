#include "CodeGenerator.hpp"

CodeGenerator::CodeGenerator()
{
	//Override by initialising the instrTable of instructions to 
	//specific language instrs and setting up initial
	//data section
}

void CodeGenerator::printLabel(string label)
{
	text << label << ":" << endl;
}

void CodeGenerator::printInstruction(Instr instr)
{
	printTab();
	text << instrTable.find(instr)->second << endl;
}

void CodeGenerator::printInstruction(Instr instr, string op)
{
	printTab();
	text << instrTable.find(instr)->second << " " << op << endl;
}

void CodeGenerator::printInstruction(Instr instr, string op1, string op2)
{
	printTab();
	text << instrTable.find(instr)->second << " " << op1 << ", " << op2 << endl;
}

void CodeGenerator::printTab()
{
	text << "\t";
}

string CodeGenerator::getAssembly()
{
	data << text.str();
	return data.str();
}
