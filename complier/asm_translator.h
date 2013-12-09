#ifndef _COMPLIER_ASM_TRANSLATOR
#define _COMPLIER_ASM_TRANSLATOR
#pragma warning(disable:4786)
#include "AsmCode.h"
#include "MemCtrl.h"
#include "SSTable.h"
#include "InsCtrl.h"
#include "Data_stream.h"


#include <IOSTREAM>
#include <STRING>
#include <SSTREAM>
#include <VECTOR>

const std::string printf_char = "$$printf_char";
const std::string printf_int = "$$printf_int";
const std::string new_line = "$$new_line";
const std::string scanf_char = "$$scanf_char";
const std::string scanf_int = "$$scanf_int";

class asm_translator  
{
public:
	asm_translator(SSTable*,InsCtrl*,Data_stream*);
	virtual ~asm_translator();
	void translate(std::ostream&);
private:
	InsCtrl* ins;
	SSTable* tbl;
	Data_stream* das;
	MemCtrl memctr;
	AsmCode obj_code;
	SymbolTable* cur_stbl;
	void write_head(std::ostream out);
	void write_body(std::ostream out);
	void write_const(std::ostream out);


	void translate(FBlock*);
	void translate(FBlock*, Quadcode*);
	
	void translate_assign(FBlock*, Quadcode*);
	void translate_inv(FBlock*, Quadcode*);
	void translate_add(FBlock*, Quadcode*);
	void translate_sub(FBlock*, Quadcode*);
	void translate_mul(FBlock*, Quadcode*);
	void translate_div(FBlock*, Quadcode*);
	
	void translate_jgt(FBlock*, Quadcode*);
	void translate_jge(FBlock*, Quadcode*);
	void translate_jlt(FBlock*, Quadcode*);
	void translate_jle(FBlock*, Quadcode*);
	void translate_jeq(FBlock*, Quadcode*);
	void translate_jne(FBlock*, Quadcode*);
	void translate_sref(FBlock*,Quadcode*);
	void translate_sind(FBlock*,Quadcode*);
	void translate_jmp(FBlock*, Quadcode*);
	void translate_fun(FBlock*, Quadcode*);
	void translate_call(FBlock*, Quadcode*);
	void translate_push(FBlock*, Quadcode*);
	void translate_param(FBlock*, Quadcode*);
	void translate_ret(FBlock*, Quadcode*);
	void translate_endf(FBlock*, Quadcode*);
	void translate_label(FBlock*, Quadcode*);
	void translate_read(FBlock*, Quadcode*);
	void translate_write(FBlock*, Quadcode*);
	void translate_new_line(FBlock*, Quadcode*);
	
	void translate_arithmetic(FBlock*, Quadcode*, asm_type);
	void translate_branch(FBlock*, Quadcode*, asm_type);




	void release_local_register(unsigned int);//
	void release_local_register(reg_name, unsigned int);//
	void premmpt_local_register(reg_name, unsigned int);
	void get_address_into_reg(reg_name,symbol*,unsigned int);
	void get_outer_add_into_reg(reg_name,symbol*, SymbolTable*);
	void occupy_local_register(symbol*, unsigned int, bool);
	void occupy_local_register(symbol*, reg_name, unsigned int, bool);
	void write_reg_back(symbol*, reg_name, unsigned int, bool, unsigned int);
	bool is_alive(symbol*, unsigned int, bool, unsigned int);
//	std::set<std::string> const_array_name; //the one array symbol name can have multiple entities
};

#endif
