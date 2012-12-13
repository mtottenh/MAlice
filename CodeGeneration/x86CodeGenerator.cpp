#include "x86CodeGenerator.hpp"
#include "../Node/NodeIncludes.hpp"

x86CodeGenerator::x86CodeGenerator()
{
	wordSize = 8;
	instrTable[ALEA] = "lea";
	instrTable[AMOVE] = "mov";
	instrTable[AMALLOC] = "malloc";
	instrTable[ASUB] = "sub";
	instrTable[AADD] = "add";
	instrTable[AIMUL] = "imul";
	instrTable[AOR] = "or";
	instrTable[AAND] = "and";
	instrTable[AXOR] = "xor";
	instrTable[ACMP] = "cmp";
	instrTable[APUSH] = "push";
	instrTable[APOP] = "pop";
	instrTable[AMOVEXTEND] = "movzx";
	instrTable[AJNE] = "jne";
	instrTable[AJE] = "je";
	instrTable[AJMP] = "jmp";
	instrTable[AINC] = "inc";
	instrTable[ADEC] = "dec";
	instrTable[ACALL] = "call";
	instrTable[ANEG] = "neg";
	instrTable[ANOT] = "not";
	instrTable[ARET] = "ret";
	instrTable[SETE] = "sete";
	instrTable[SETLE] = "setle";
	instrTable[SETL] = "setl";
	instrTable[SETG] = "setg";
	instrTable[SETGE] = "setge";
	instrTable[SETNE] = "setne";
}

void x86CodeGenerator::printInstruction(Instr instr, string ret, string nxt)
{
	switch(instr)	
	{
		case ADIV:
				text << "\tpush rax" << endl;                              
				text << "\tpush rdx" << endl;                             
				/* not sure if rax or rdx... see intel documentation */
				text << "\txor rdx, rdx" << endl;
				text << "\tmov " << "rax, " << ret << endl;
				text << "\tcqo" << endl;
				text << "\tidiv " << nxt << endl;
				/* quotient lives in rax, move to resultReg */
				text << "\tmov " << ret << ", rax" << endl;
				text << "\tpop rdx" << endl;
				text << "\tpop rax" << endl;         
			break;
		case AMOD:
				text << "\tpush rax" << endl;                              
				text << "\tpush rdx" << endl;                             
				/* not sure if rax or rdx... see intel documentation */
				text << "\txor rdx, rdx" << endl;
				text << "\tmov " << "rax, " << ret << endl;
				text << "\tcqo" << endl;
				text << "\tidiv " << nxt << endl;
				/* quotient lives in rax, move to resultReg */
				text << "\tmov " << ret << ", rdx" << endl;
				text << "\tpop rdx" << endl;
				text << "\tpop rax" << endl;
			break;
		default:
				CodeGenerator::printInstruction(instr, ret, nxt); 
	}
}

void x86CodeGenerator::printInitial()
{
	data << "%include\t\'system.inc\'\n\n";
	data << "section .data\n\n";  
}

void x86CodeGenerator::printData(string label, string size)
{
	data << "common\t" << label << " " << size << endl;
}

void x86CodeGenerator::printStringLitData(string label, string s)
{
	data << label << ": db " << s << ",0x0" << endl;
}

void x86CodeGenerator::printExtern()
{
	text << "section .text\n";                                             
	text << "global main\n";                                               
	text << "extern exit\n";                                               
	text << "extern scanf\n";                                              
	text << "extern printf\n";  
	text << "extern malloc\n";                                          
	text << "main:\n";  
}

deque<string> x86CodeGenerator::getAllGeneralRegs()
{
	deque<string> allRegs;
	allRegs.clear();
 	allRegs.push_back("r8");                                            
	allRegs.push_back("r9");                                         
	allRegs.push_back("r10");                                          
	allRegs.push_back("r11");                                          
	allRegs.push_back("r12");                                          
	allRegs.push_back("r13");                                           
	allRegs.push_back("r14");                                           
	allRegs.push_back("r15");                                           
	allRegs.push_back("rbx");                                           
	allRegs.push_back("rcx");                                           
	allRegs.push_back("rdx");                                           
	allRegs.push_back("rsi");                                           
	allRegs.push_back("rdi");	
	return allRegs;
}

string x86CodeGenerator::getBasePointer()
{
	return "rbp";
}

string x86CodeGenerator::getStackPointer()
{
	return "rsp";
}

string x86CodeGenerator::getReturnRegister()
{
	return "rax";
}

void x86CodeGenerator::generatePrintInstruction
						(string reg, string label, bool ref, int type)
{
	switch(type)
	{
		case TCHAR:
			printStringLitData(label, "\"%c\"");
			break;
		case TNUMBER:
			printStringLitData(label, "\"%d\"");
			break; 
		case TSTRING:
			printStringLitData(label, "\"%s\"");
		//TODO: ADD MORE CASES! 
	}
	printInstruction(APUSH, "r9");
	printInstruction(APUSH, "r8");
	printInstruction(APUSH, "rax");
	printInstruction(APUSH, "rdi");
	printInstruction(APUSH, "rsi");

	if (reg.size() != 0)
	{
		if (ref)
		{
			printInstruction(AMOVE, "rsi", "[" + reg + "]");
		}                                             
		else
		{
			printInstruction(AMOVE, "rsi", reg);
		}
	}
	printInstruction(AMOVE, "rdi", label);
	printInstruction(AMOVE, "rax", "0");
	printInstruction(ACALL, "printf");
	                                   
	printInstruction(APOP, "rsi");
	printInstruction(APOP, "rdi");
	printInstruction(APOP, "rax");
	printInstruction(APOP, "r8");
	printInstruction(APUSH, "r9");
}

void x86CodeGenerator::printReturnFromProgram()
{
	printInstruction(AMOVE, "rdi", "0");
	printInstruction(ACALL, "exit");
}

void x86CodeGenerator::comparePredicate(Instr instr, string res, string nxt)
{
	printInstruction(ACMP, res, nxt);
	printInstruction(APUSH, "rax");
	printInstruction(instr, "al");
	printInstruction(AMOVEXTEND, res, "al");
	printInstruction(APOP, "rax"); 
}

void x86CodeGenerator::generateAccessInstruction(string addrReg, 
							string offset, string result, string arrLabel)
{
	printInstruction(AMOVE, addrReg, arrLabel);          
	printInstruction(AIMUL, offset, "8");                           
	printInstruction(AADD, addrReg, offset);                        
	printInstruction(ALEA, result, "[" + addrReg + "]");  
}

void x86CodeGenerator::printHeapAllocationInstruction(string label, 
											string numInstr)
{
	printInstruction(APUSH, "rax");                         
	printInstruction(APUSH, "rdi");                         
	printInstruction(AMOVE, "rdi", numInstr);   
	printInstruction(AXOR, "rax", "rax");                    
	printInstruction(ACALL, "malloc");
	printInstruction(AMOVE, "[" + label + "]", "rax");      
	printInstruction(APOP, "rdi");                          
	printInstruction(APOP, "rax");                          
	printData(label, "8"); 
}

void x86CodeGenerator::generateInputFunction(string scanf, string addr)
{
	printStringLitData(scanf, "\"%d\"");
	text << "\tpush rdi\n\tpush rax\n";                               
	text << "\tmov rdi, " << scanf << endl;   
	text << "\tlea rsi , " << addr << "\n";  
	text << "\tmov rax, 0\n"; 
	text << "\tmov qword " << addr << ", 0\n";
	text << "\tcall scanf" << endl; 
	text << "\tmovsx rax, dword " << addr << "\n";
	text << "\tmov " << addr << ", rax\n";
	text << "\tpop rax" << endl;                                      
	text << "\tpop rdi" << endl;        
}

string x86CodeGenerator::generateMemoryOffset(string reg, int elements)
{
	return "[" + reg + boost::lexical_cast<string>(elements * wordSize) + "]";
}

