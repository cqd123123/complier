#pragma warning(disable:4786)
#include "AsmCode.h"

const static std::string CODE_MAP[]={
		"LABEL",
		"MOV",
		"CDQ",// The CDQ instruction copies the sign (bit 31) of the value in the EAX register into every bit position in the EDX register.
		"NEG",//negate
		"ADD",
		"SUB",
		"IMUL",//integer mul
		"IDIV",//..
		"CMP",
		"TEST",//logical compare, performs a bitwise AND on two operands
		"JG", //jump greater than
		"JGE",
		"JL",
		"JLE",
		"JE",
		"JNE", 
		"JP", //jump parity
		"JNP",//jump not parity
		"JMP",
		"PUSH",//push into stack will change esp
		"POP",
		"CALL",//the return address will be pushed into the stack
		"PROC",//begin procedure, eg Sum of PROC add eax, ebx add eax, ecx ret Sumof ENDP
		"ENDP",
		"RET",//save return address in the stack, also argumennts
		"END"
};


asm_ins::asm_ins(asm_type _ins):oper(_ins)
{
	enable = true;
	args = 0;
}
asm_ins::asm_ins(asm_type _ins, std::string _oper1):oper(_ins), oper1(_oper1)
{
	enable = true;
	args = 1;
}
asm_ins::asm_ins(asm_type _ins, std::string _oper1, std::string _oper2):oper(_ins), oper1(_oper1), oper2(_oper2)
{
	enable = true;
	args = 2;
}
void AsmCode::add(asm_type typ)
{
	asm_ins* ins=new asm_ins(typ);
	all_codes.push_back(ins);
}
void AsmCode::add(asm_type typ,std::string arg)
{
	asm_ins* ins=new asm_ins(typ,arg);
	all_codes.push_back(ins);
}
void AsmCode::add(asm_type typ,std::string arg1,std::string arg2)
{
	asm_ins* ins=new asm_ins(typ,arg1,arg2);
	all_codes.push_back(ins);
}
void AsmCode::Peephole()
{
	asm_ins* prev_code=all_codes[0];
	for(int i=1;i<all_codes.size();i++)
	{
		asm_ins* cur_code=all_codes[i];
		if(prev_code->oper==MOV&&cur_code->oper==MOV)	//two move have same obj
		{
			if(prev_code->oper1==cur_code->oper1)
				prev_code->enable=false;
		}
		prev_code=cur_code;
	}
}
std::ostream& operator<<(std::ostream& out, const asm_ins& ins)
{
	if(!ins.enable) return out;
	switch(ins.args)
	{
		case 0:
			out<<CODE_MAP[ins.oper]<<std::endl;
			break;
		case 1:
			if(ins.oper==PROC||ins.oper==ENDP)
			{ out<<ins.oper1<<" "<<CODE_MAP[ins.oper]<<std::endl; if(ins.oper==ENDP) out<<std::endl;}
			else if(ins.oper==LABEL)
				out<<ins.oper1<<":"<<std::endl;
			else
				out<<CODE_MAP[ins.oper]<<" "<<ins.oper1<<std::endl;
			break;
		case 2:
			out<<CODE_MAP[ins.oper]<<" "<<ins.oper1<<","<<" "<<ins.oper2<<std::endl;
			break;
	}
	return out;
}
std::ostream& operator<<(std::ostream& out, const AsmCode& ins)
{
	out << ".code" << std::endl;
	for(unsigned int i = 0; i <ins.all_codes.size(); i++)
	{
		out << *(ins.all_codes[i]);
	}
	return out;
}
std::vector<asm_ins*> AsmCode::get_all_ins() const
{
	return all_codes;
}