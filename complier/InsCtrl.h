#ifndef _COMPILER_INSCTRL
#define _COMPILER_INSCTRL
#pragma warning(disable:4786)
#include "token.h"
#include "SSTable.h"
#include "Quadcode.h"
#include "FBlock.h"
#include "BBlock.h"

#include <VECTOR>
#include <IOSTREAM>
#include <MAP>
#include <SET>

class InsCtrl  
{

public:
	friend class Dag;
	InsCtrl(SSTable&);

	void arithmetic_operation(symbol*, token, symbol*, symbol*&);
	void inverse(symbol*, symbol*&);
	void assign(symbol*, symbol*);
	void assign(symbol*, symbol*, token, symbol*);

	void add_label(symbol*);
	void jmp(symbol*);
	void branch(symbol*, token, symbol*, symbol*&);

	void fun(symbol*);//the function will take some memory for the return value while the proc do not
	void proc(symbol*);
	void param(symbol*);
	void set_ref(symbol*);
	void set_index(symbol*,symbol*, bool back);
	void call(symbol*);
	void call(symbol*, symbol*&);
	void push(symbol*);//add params
	void ret(symbol*, symbol*);//
	void ret(symbol*);
	void end_fun(symbol*);
	void end_proc(symbol*);

	void read(symbol*);
	void write(symbol*);

	void shift_line();
	void main_begin(symbol*);
	void main_end(symbol*);
	std::vector<Quadcode*> get_all_ins() const;
	void print_all_ins();
	void reorder_all_ins();
	SSTable get_ss_tabel(){return sstab;}
	std::vector<FBlock*> get_all_func_block();
private:
	class code_block{
	public: code_block(){};
			std::vector<Quadcode*> codes;
			void add_ins(Quadcode* ins){codes.push_back(ins);}
	};
	void new_instruction(Quadcode*);
	std::vector<Quadcode*> Quad_inss;
	SSTable& sstab;
	void update_reference(Quadcode*);
	std::vector<FBlock*> func_block;
	std::vector< code_block* > stack_code_block;
	std::vector< code_block* > all_code_block;
};

#endif
