#include "ARMCodeGenerator.hpp"
#include "../Node/NodeIncludes.hpp"

ARMCodeGenerator::ARMCodeGenerator()
{
	instrTable[ALEA] = "lea"; /* Maybe not needed? */
	instrTable[AMOVE] = "mov";
	instrTable[ALDR] = "ldr" /* Additional ARM instruction */
	instrTable[AMALLOC] = "malloc";
	instrTable[ASUB] = "sub";
	instrTable[AADD] = "add";
	instrTable[AIMUL] = "mul";
	instrTable[AOR] = "or";
	instrTable[AAND] = "and";
	instrTable[AXOR] = "xor";
	instrTable[ACMP] = "cmp";
	instrTable[APUSH] = "push";
	instrTable[APOP] = "pop";
	instrTable[AMOVEXTEND] = "movzx"; /* Maybe not needed? */
	instrTable[AJNE] = "bne";
	instrTable[AJE] = "be";
	instrTable[AJMP] = "b";
	instrTable[AINC] = "inc";
	instrTable[ADEC] = "dec";
	instrTable[ACALL] = "bl";
	instrTable[ANEG] = "neg";
	instrTable[ANOT] = "mvn";
	instrTable[ARET] = "ret"; /* Not needed - no ret in ARM! */
	instrTable[SETE] = "sete";
	instrTable[SETLE] = "setle";
	instrTable[SETL] = "setl";
	instrTable[SETG] = "setg";
	instrTable[SETGE] = "setge";
	instrTable[SETNE] = "setne";
}

void ARMCodeGenerator::printInstruction(Instr instr, string ret, string nxt)
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

void ARMCodeGenerator::printInitial()
{
	data << "%include\t\'system.inc\'\n\n";
	data << "section .data\n\n";  
}

void ARMCodeGenerator::printData(string label, string size)
{
	data << "common\t" << label << " " << size << endl;
}

void ARMCodeGenerator::printStringLitData(string label, string s)
{
	data << label << ": db " << s << ",0x0" << endl;
}

void ARMCodeGenerator::printExtern()
{
	text << "section .text\n";                                             
	text << "global main\n";                                               
	text << "extern exit\n";                                               
	text << "extern scanf\n";                                              
	text << "extern printf\n";  
	text << "extern malloc\n";                                          
	text << "main:\n";  
}

deque<string> ARMCodeGenerator::getAllGeneralRegs()
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

string ARMCodeGenerator::getBasePointer()
{
	return "rbp";
}

string ARMCodeGenerator::getStackPointer()
{
	return "rsp";
}

string ARMCodeGenerator::getReturnRegister()
{
	return "rax";
}

void ARMCodeGenerator::generatePrintInstruction
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

void ARMCodeGenerator::printReturnFromProgram()
{
	printInstruction(AMOVE, "rdi", "0");
	printInstruction(ACALL, "exit");
}

void ARMCodeGenerator::comparePredicate(Instr instr, string res, string nxt)
{
	printInstruction(ACMP, res, nxt);
	printInstruction(APUSH, "rax");
	printInstruction(instr, "al");
	printInstruction(AMOVEXTEND, res, "al");
	printInstruction(APOP, "rax"); 
}

void ARMCodeGenerator::generateAccessInstruction(string addrReg, 
							string offset, string result, string arrLabel)
{
	printInstruction(AMOVE, addrReg, arrLabel);          
	printInstruction(AIMUL, offset, "8");                           
	printInstruction(AADD, addrReg, offset);                        
	printInstruction(ALEA, result, "[" + addrReg + "]");  
}

void ARMCodeGenerator::printHeapAllocationInstruction(string label, 
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

void ARMCodeGenerator::generateInputFunction(string scanf, string addr)
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
