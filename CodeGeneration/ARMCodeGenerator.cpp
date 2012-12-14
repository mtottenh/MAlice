#include "ARMCodeGenerator.hpp"
#include "../Node/NodeIncludes.hpp"
#include <boost/lexical_cast.hpp>

ARMCodeGenerator::ARMCodeGenerator()
{
	allRegs.clear();	
	wordSize = 4;
	instrTable[ALEA] = "lea"; /* Maybe not needed? */
	instrTable[AMOVE] = "mov";
	instrTable[ALDR] = "ldr"; /* Additional ARM instruction */
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
	instrTable[AMOD] = "mod";
	instrTable[ADIV] = "div";
	instrTable[ARET] = "ret"; /* Not needed - no ret in ARM! */
	instrTable[SETE] = "moveq";
	instrTable[SETLE] = "movle";
	instrTable[SETL] = "movl";
	instrTable[SETG] = "movg";
	instrTable[SETGE] = "movge";
	instrTable[SETNE] = "movne";
}

void ARMCodeGenerator::printInstruction(Instr instr, string ret, string nxt)
{
	switch(instr)	
	{
		case AMOVE:
				evalMove(ret, nxt);
				break;
		case ASUB:
				CodeGenerator::printInstruction(instr, ret, "#" + nxt);
				break;
		default:
				CodeGenerator::printInstruction(instr, ret, nxt); 
	}
}

void ARMCodeGenerator::evalMove(string ret, string nxt) {
	/* Is the first char of the return val a memory address? */
	if(ret[0] == '[') {
		string addrReg = ret.substr(1, ret.length()-2);

		/* Is the next reg a register? i.e. mov [sp+4], r2 */
		if(isReg(nxt)) {
			
			printInstruction(APUSH, "r0, r1");
			text << "\tmov r0, " << addrReg << endl;
			text << "\tstr " << nxt << ", [r0]" << endl;
			printInstruction(APOP, "r0, r1");
		}

		/* Okay, so next is immediate. Same as above but ldr the imm value. */
		else {
			printInstruction(APUSH, "r2");
			text << "\tldr r2, =" << nxt << endl;
			evalMove(ret, "r2");
			printInstruction(APOP, "r2");
		}
			
		}

	/* ret is a register */
	else {
		/* Is nxt a register? */
		if(isReg(nxt)) {
			text << "\tmov " << ret << ", " << nxt << endl;
		}

		/* Is nxt a memory access? */
		else if(nxt[0] == '[') {
			text << "\tldr " << ret << ", " << nxt << endl;
		}

		/* Otherwise immediate. */
		else {
			text << "\tldr " << ret << ", =" << nxt << endl;
		}
	}
}

int ARMCodeGenerator::isReg(string reg) {
	return (reg == "sp" || reg == "pc" || reg == "r0" || reg == "r12" ||
				find(allRegs.begin(), allRegs.end(), reg) != allRegs.end());
}

void ARMCodeGenerator::printInstruction(Instr instr, string reg) {
	switch(instr) {
		case APUSH:
			text << "\t" << instrTable[APUSH] << " {" << reg << "}" << endl;
			break;
		case APOP:
			text << "\t" << instrTable[APOP] << " {" << reg << "}" << endl;
			break;
		default:
			CodeGenerator::printInstruction(instr, reg);
	}
}

void ARMCodeGenerator::printInitial()
{
	data << ".data" << endl;  
}

void ARMCodeGenerator::printData(string label, string size)
{
	data << ".comm\t" << label << " " << size << endl;
}

void ARMCodeGenerator::printStringLitData(string label, string s)
{
	data << label << ":\n\t .asciz " << s << endl;
}

void ARMCodeGenerator::printExtern()
{
	text << ".text\n";                                             
	text << ".global main\n";                                               
	text << "main:\n";  
}

deque<string> ARMCodeGenerator::getAllGeneralRegs()
{
	if(this->allRegs.empty()) {
		allRegs.push_back("r1");
		allRegs.push_back("r2");                                         
		allRegs.push_back("r3");                                          
		allRegs.push_back("r4");                                          
		allRegs.push_back("r5");                                          
		allRegs.push_back("r6");                                           
		allRegs.push_back("r7");                                           
		allRegs.push_back("r8");                                           
		allRegs.push_back("r9");                                           
		allRegs.push_back("r10");                                           
		allRegs.push_back("r11");
	}

	return allRegs;
}

string ARMCodeGenerator::getBasePointer()
{
	return "r12";
}

string ARMCodeGenerator::getStackPointer()
{
	return "sp";
}

string ARMCodeGenerator::getReturnRegister()
{
	return "r0";
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
	}
	printInstruction(APUSH, "r0");
	printInstruction(APUSH, "r1");
	printInstruction(APUSH, "r2");
	printInstruction(APUSH, "r3");
	printInstruction(APUSH, "r4");

	if (reg.size() != 0)
	{
		if (ref)
		{
			printInstruction(ALDR, "r1", "[" + reg + "]");
		}                                             
		else
		{
			printInstruction(AMOVE, "r1", reg);
		}
	}
	printInstruction(ALDR, "r0", "=" + label);
	printInstruction(ACALL, "printf");
	                                   
	printInstruction(APOP, "r4");
	printInstruction(APOP, "r3");
	printInstruction(APOP, "r2");
	printInstruction(APOP, "r1");
	printInstruction(APOP, "r0");
}

void ARMCodeGenerator::printReturnFromProgram()
{
	printInstruction(ALDR, "r0", "=0");
	printInstruction(AJMP, "exit");
}

void ARMCodeGenerator::comparePredicate(Instr instr, string res, string nxt)
{
	printInstruction(APUSH, "r0");
	printInstruction(AMOVE, "r0", "0");
	printInstruction(ACMP, res, nxt);
	printInstruction(instr, res, "1");
	printInstruction(AMOVE, res, "r0");
	printInstruction(APOP, "r0");
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

string ARMCodeGenerator::generateMemoryOffset(string reg, int numElements)
{
	return "[" + reg + ", #" + 
			boost::lexical_cast<string>(numElements * wordSize) + "]";
}
