#pragma warning(disable:4786)
#include "SSTable.h"
#include <CASSERT>
#include <SSTREAM>
#include <IOSTREAM>

unsigned int SSTable:: funcNum = 1;
SSTable::SSTable()
{
	before_table=NULL;
	label_count=0;
}
bool SSTable::add_symbol(symbol::symbol_value_type typ, std::string _id)//variable
{
	symbol* s=symbol::var_symbol(_id,typ);
	return add_s(s);
}
bool SSTable::add_symbol(symbol::symbol_value_type typ, std::string _id, std::string _val)//const
{
	symbol* s=symbol::const_symbol(_id,_val,typ);
	return add_s(s);
}
bool SSTable::add_array(symbol::symbol_value_type typ, std::string _id , unsigned int size)
{
	symbol* s=symbol::array_symbol(_id,size,typ);
	return add_s(s);
}
void SSTable::add_literal(symbol*& lit)
{
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	assert(lit->get_sym_type()==symbol::LITERAL);
	if(lit->get_val_type()==symbol::CHAR)
	{
		std::string _val;
		ss<<lit->get_value();
		ss>>_val;
		std::map<std::string, symbol*>::iterator it=char_literals.find(_val);
		if(it!=char_literals.end())
		{
			delete lit;
			lit=it->second;
		}
		else
			char_literals[_val]=lit;
	}
	if(lit->get_val_type()==symbol::INTEGER)
	{	
		int _val;
		ss<<lit->get_value();
		ss>>_val;
		std::map<int, symbol*>::iterator it=int_literals.find(_val);
		if(it!=int_literals.end())
		{
			delete lit;
			lit=it->second;	//	std::cout<<*lit;
		}
		else
			int_literals[_val]=lit;
	}
	if(lit->get_val_type()==symbol::STRING)
	{
		std::string _val;
		_val=lit->get_value();
		std::map<std::string, symbol*>::iterator it=string_literals.find(_val);
		if(it!=string_literals.end())
		{
			delete lit;
			lit=it->second;
		}
		else
			string_literals[_val]=lit;
	}
}
bool SSTable::add_argument(symbol::symbol_value_type typ, std::string _id , symbol::arguments* args,bool is_ref)
{
	symbol* s=NULL;
	if(!is_ref)
		s=symbol::argument_symbol(_id, typ);
	else
		s=symbol::ref_symbol(_id,typ);
	if(add_s(s))
	{
		args->push_back(s);
		return true;
	}
	else
		return false;
}
bool SSTable::add_symbol(symbol::symbol_value_type typ, std::string _id, symbol::arguments* args, symbol*& sym)
{
	sym=symbol::func_symbol(_id,typ,args,funcNum++);
	return add_s(sym);
}
bool SSTable::add_symbol(std::string _id,symbol::arguments* args,symbol*& sym)
{	
	sym=symbol::proc_symbol(_id,args,funcNum++);
	return add_s(sym);
}
void SSTable::pop_symbol_table()
{
	stack_s_table.pop_back();
}
void SSTable::new_symbol_table(symbol* label)
{
	SymbolTable* tab=new SymbolTable(label);
	if(stack_s_table.size()>=1)
		tab->set_before_table(stack_s_table.back());
	stack_s_table.push_back(tab);
	sym_table_map[label]=tab;
}
symbol* SSTable::get_current_scope() const//get current scope
{
	return stack_s_table.back()->get_scope();
}
void  SSTable::new_temp(symbol*& temps, symbol::symbol_value_type typ)
{
	temps=stack_s_table.back()->get_next_temp(typ);
}
bool SSTable::is_outer(symbol* sym)
{
	int lev1,lev2;
	symbol::symbol_type typ;
	typ=sym->get_sym_type();
	if(typ!=symbol::LITERAL&&typ!=symbol::CONST&&!sym->is_global())
		lev1=sym->get_scope()->get_nested_lev();
	else return false;
	lev2=stack_s_table.back()->get_nested_lev();
	return lev1<lev2;
}
void SSTable::new_label(symbol*& sym)
{
	//sym=stack_s_table.back()->get_next_label();
	symbol* sys;
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << "label" << label_count++;
	sys=symbol::label_symbol(ss.str());
	sys->set_scope(stack_s_table.back());
	sym=sys;
}
bool SSTable::add_s(symbol* s)
{
	return stack_s_table.back()->add_symbol(s);
}
SymbolTable* SSTable::get_table(symbol* sym) const
{
	return sym_table_map.find(sym)->second;
}
symbol* SSTable::look_up(std::string _id) const
{
	symbol* sym;
	for(std::vector<SymbolTable*>::const_reverse_iterator it=stack_s_table.rbegin();it!=stack_s_table.rend();it++)
		if((*it)->find_symbol(_id, sym))
			return sym;
	return NULL;
}
std::map<symbol*, SymbolTable*> SSTable::get_symbol_tables() const
{
	return sym_table_map;
}
void SSTable::DumpSymBols() const
{
	std::cout<<"---------------------------------------"<<std::endl;
	std::cout<<"size of tables"<<stack_s_table.size()<<std::endl;
	for(int i=0;i<stack_s_table.size();i++)
	{
		SymbolTable* s_tbl=stack_s_table[i];
		while(s_tbl!=NULL)
		{
			std::cout<<s_tbl->get_scope()->get_id()<<"@"<<s_tbl->get_scope()->get_index()<<"-";
			s_tbl=s_tbl->get_before_table();
		}
		std::cout<<"||\n";
	}
	std::cout<<"*****************"<<std::endl;
	for(std::vector<SymbolTable*>::const_reverse_iterator it=stack_s_table.rbegin();it!=stack_s_table.rend();it++)
	{   
		std::vector<symbol*> all_sym=(*it)->get_all_symbol();
		symbol* scop=(*it)->get_scope();
		std::cout<<*scop;
		for(std::vector<symbol*>::iterator it=all_sym.begin();it!=all_sym.end();it++)
			std::cout<<*(*it);
		std::cout<<"*****************"<<std::endl;
	}
	std::cout<<"string lit-----"<<std::endl;
	for(std::map<std::string,symbol*>::const_iterator iter=string_literals.begin();iter!=string_literals.end();iter++)
	{
		std::cout<<*iter->second;
	}
	std::cout<<"char lit-----"<<std::endl;
	for(std::map<std::string,symbol*>::const_iterator iter1=char_literals.begin();iter1!=char_literals.end();iter1++)
	{
		std::cout<<*iter1->second;
	}
	std::cout<<"int lit-----"<<std::endl;
	for(std::map<int,symbol*>::const_iterator iter3=int_literals.begin();iter3!=int_literals.end();iter3++)
	{
		std::cout<<*iter3->second;
	}
	std::cout<<"---------------------------------------"<<std::endl;
}