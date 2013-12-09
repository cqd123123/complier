#ifndef _COMPILER_SSTABLE
#define _COMPILER_SSTABLE
#pragma warning(disable:4786)
#include "SymbolTable.h"
#include "symbol.h"
#include <MAP>
#include <VECTOR>


class SSTable  
{
public:
	SSTable();

	bool add_symbol(symbol::symbol_value_type, std::string, std::string);//const
	bool add_symbol(symbol::symbol_value_type, std::string);//variable
	bool add_array(symbol::symbol_value_type,std::string,unsigned int size);

	bool add_symbol(symbol::symbol_value_type, std::string, symbol::arguments*, symbol*&);//function
	bool add_symbol(std::string, symbol::arguments* , symbol*&);//procedure

	bool add_argument(symbol::symbol_value_type, std::string, symbol::arguments*, bool);	

	void add_literal(symbol*&);

	void new_temp(symbol*&, symbol::symbol_value_type);//

	void new_label(symbol*&);//

	symbol* look_up(std::string) const;//

	symbol* get_current_scope() const;//

	SymbolTable* get_table(symbol* sym) const;//

	std::map<symbol*, SymbolTable*> get_symbol_tables() const;//
	
	void new_symbol_table(symbol*);//
	
	void pop_symbol_table();//
	bool is_outer(symbol*);

	void allocate();
	void DumpSymBols() const;
	static unsigned int funcNum;
private:
	bool add_s(symbol*);
	std::map<symbol*,SymbolTable*> sym_table_map;
	std::vector<SymbolTable*> stack_s_table;
	std::map<int, symbol*> int_literals;
	std::map<std::string, symbol*> char_literals;
	std::map<std::string, symbol*> string_literals;
	unsigned int label_count;
	SymbolTable* before_table;
};

#endif