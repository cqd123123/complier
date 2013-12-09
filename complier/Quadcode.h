#ifndef _COMPILER_QUADCODE
#define _COMPILER_QUADCODE
#pragma warning(disable:4786)
#include "symbol.h"
#include "SymbolTable.h"
#include <VECTOR>

class Quadcode  
{
public:
	const enum Quad_type
	{
		ADD,	//
		ASSIGN,	//
		MUL,	//
		SUB,	//
		DIV,//divide
		INV,//inverse
		CALL,//call
		FUNC,
		PROC,	//not uesd
		JMP,
		JGT,//jump greater than
		JGE,//jump great equal
		JEQ,//jump eq
		JNE,
		JLE,//jump less equal
		JLT,//jump less than
		LABEL,
		READ,
		WRITE,
		PARAM,//parameter
		SREF,//set reference
		ENDF,
		ENDP,
		RET,
		PUSH,
		POP,
		MAIN,//main proc begin
		ENDM,//whole proc end
		SIND,
		LINE,
	};
	Quadcode();
	static Quadcode* new_Quad_code(Quad_type,symbol*, symbol* , symbol*);//arithmetic
	static Quadcode* new_Quad_code(Quad_type,symbol*,symbol*);//inverse, etc
	static Quadcode* new_Quad_code(Quad_type,symbol*);//jmp label
	static Quadcode* new_Quad_code(Quad_type);//fun,proc..

	unsigned int get_index() const;
	void set_index(unsigned int index);
	unsigned int get_args_size() const;
	Quad_type get_quad_type() const;

	bool is_enable() const;
	void disable();

	symbol* get_obj_arg();
	std::vector<symbol*> get_src_arg();
	std::vector<symbol*> get_all_sym();
	static void reset();
	friend void operator<<(std::ostream&, const	Quadcode&);
private:
	Quad_type type;
	symbol* obj;
	std::vector<symbol*> src;
	unsigned int index;
	std::vector<symbol*> symbols;
	static unsigned int counter;

	bool enable;
};
#endif

