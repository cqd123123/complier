#ifndef _COMPILER_ASMCODE
#define _COMPLIER_ASMCODE
#pragma warning(disable:4786)
#include <STDIO.H>
#include <VECTOR>
#include <STRING>

const static enum asm_type{
		LABEL,
		MOV,
		CDQ,// The CDQ instruction copies the sign (bit 31) of the value in the EAX register into every bit position in the EDX register.
		NEG,//negate
		ADD,
		SUB,
		IMUL,//integer mul
		IDIV,//..
		CMP,
		TEST,//logical compare, performs a bitwise AND on two operands
		JG, //jump greater than
		JGE,
		JL,
		JLE,
		JE,
		JNE, 
		JP, //jump parity
		JNP,//jump not parity
		JMP,
		PUSH,//push into stack will change esp
		POP,
		CALL,//the return address will be pushed into the stack
		PROC,//begin procedure, eg Sum of PROC add eax, ebx add eax, ecx ret Sumof ENDP
		ENDP,
		RET,//save return address in the stack, also argumennts
		END
};
struct asm_ins
{
public:
	friend class AsmCode;
	asm_ins(asm_type);
	asm_ins(asm_type, std::string);
	asm_ins(asm_type, std::string, std::string);
	friend std::ostream& operator<<(std::ostream&, const asm_ins&);
private:
	asm_type oper;
	std::string oper1;
	std::string oper2;
	bool enable;
	int args;
};
class AsmCode  
{
public:
	void add(asm_type);
	void add(asm_type, std::string);
	void add(asm_type, std::string, std::string);

	void Peephole();

	std::vector<asm_ins*> get_all_ins() const;	
	friend std::ostream& operator<<(std::ostream&, const AsmCode&);
private:
	std::vector<asm_ins*> all_codes;
};
#endif