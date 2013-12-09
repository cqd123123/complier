#ifndef _COMPILER_SYMBOLTABLE
#define _COMPILER_SYMBOLTABLE
#pragma warning(disable:4786)
#include <MAP>
#include <VECTOR>
#include <STRING>
#include "symbol.h"

class SymbolTable  
{
public:
	SymbolTable(){};
	SymbolTable(symbol*);
	symbol* get_next_label();
	symbol* get_next_temp(symbol::symbol_value_type);
	bool add_symbol(symbol*);
	bool find_symbol(std::string,symbol*&);
	symbol* get_scope();
	int get_nested_lev();
	void update_reference(symbol*, int);
	std::vector<symbol*> get_all_symbol();

	SymbolTable* get_before_table();
	void set_before_table(SymbolTable*);
	void Dump_all_symbols();
	std::string get_name();
private:
	symbol* scope;
	std::map<std::string, symbol*> symbol_map;
	std::map<symbol*,int> symbol_ref;
	std::vector<symbol*> symbol_all;
	unsigned int label_count;
	unsigned int temp_count;
	SymbolTable* before_table;
	int nested_lev;
};

#endif
