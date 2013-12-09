#pragma warning(disable:4786)
#include "SymbolTable.h"
#include <sstream>
#include <IOSTREAM>
#include <CASSERT>

SymbolTable::SymbolTable(symbol* scop)
{
	scope=scop;
	label_count=0;
	temp_count=0;
	nested_lev=0;
	before_table=NULL;
	if(scop->get_sym_type()==symbol::PROCEDURE&&scop->get_index()==0)//set the scope for the first symbol spawn the symbol table
	{scop->set_scope(this);scop->set_cur_scope(this);}
	else
		scop->set_cur_scope(this);
}

bool SymbolTable::add_symbol(symbol* sym)
{
	symbol::symbol_type typ;
	if(sym==NULL)
		return false;
	typ=sym->get_sym_type();
	if(typ==symbol::VARIABLE||typ==symbol::ARGUMENT||typ==symbol::CONST||
		typ==symbol::FUNCTION||typ==symbol::PROCEDURE||typ==symbol::ARRAY||typ==symbol::REFSYM)
	{
		std::string name;
		name=sym->get_id();
		if(symbol_map.find(name)!=symbol_map.end())
			return false;
		else
		{
			symbol_map[sym->get_id()]=sym;
			symbol_all.push_back(sym);
		
			sym->set_scope(this);
			return true;
		}
	}
	else
		return false;
}
bool SymbolTable::find_symbol(std::string name,symbol*& sym)
{
	if(symbol_map.find(name)!=symbol_map.end())
	{
		sym=symbol_map.find(name)->second;
		return true;
	}
	else
		return false;
}
symbol* SymbolTable::get_next_label()
{
	symbol* sys;
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << "label" << label_count++;
	sys=symbol::label_symbol(ss.str());
	sys->set_scope(this);
	return sys;
}
symbol* SymbolTable::get_next_temp(symbol::symbol_value_type _type)
{
	symbol* sys;
	std::stringstream ss(std::stringstream::in|std::stringstream::out);
	ss<<"$temp"<<temp_count++;
	sys=symbol::var_symbol(ss.str(),_type);
	add_symbol(sys);
	return sys;
}
symbol* SymbolTable::get_scope()
{
	return scope;
}
void SymbolTable::update_reference(symbol* sym, int ref)
{
	symbol_ref[sym]+=ref;
	if(symbol_ref[sym]<=0) sym->set_enable(false);
	else sym->set_enable(true);
	assert(symbol_ref[sym]>=0);
}

std::vector<symbol*> SymbolTable::get_all_symbol()
{
	return symbol_all;
}
SymbolTable* SymbolTable::get_before_table()
{
	return before_table;
}
void SymbolTable::set_before_table(SymbolTable* before)
{
	before_table=before;
	SymbolTable* ttbl;
	ttbl=this;
	while(ttbl->get_name()!="@global")
	{
		nested_lev++;
		ttbl=ttbl->get_before_table();
	}
	if(nested_lev>0) nested_lev--;
}
int SymbolTable::get_nested_lev()
{
	return nested_lev;
}
std::string SymbolTable::get_name()
{
	return this->scope->get_id();
}
void SymbolTable::Dump_all_symbols()
{
	std::cout<<"-------------------"<<(this->get_scope())<<std::endl;
	for(int i=0;i<symbol_all.size();i++)
		std::cout<<*symbol_all[i];
	std::cout<<"---------------------"<<std::endl;
}