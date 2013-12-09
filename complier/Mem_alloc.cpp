#pragma warning(disable:4786)
#include "Mem_alloc.h"
#include "MemCtrl.h"
#include <STRING>
#include "utility.h"
#include <fstream>

Mem_alloc::Mem_alloc(SymbolTable* tbl, FBlock* _fblk, C_graph* grap, Live_Var* _l_var):fblk(_fblk),l_var(_l_var)
{
	if(grap!=NULL)
	{
		webs=grap->get_Webs();
		alloc_global_reg(grap);
	}
	function_tbl=tbl;	
	allocate_mem();
	nested_level=tbl->get_nested_lev();

	std::ofstream file("t_global_reg.txt",std::ios::app);
	for(std::map<web*, reg_name>::iterator it=global_reg_map.begin();it!=global_reg_map.end();it++)
	{
		file<<*webs->get_symbol(it->first);
		file<<it->second<<std::endl;
	}
	file.close();
}

reg_name Mem_alloc::in_register(symbol* sym, unsigned int line, bool use) const		//whether a given symbol is in global regs
{
	if(global_reg_map.size()==0)
		return NA;
	else
	{
		web* webb=webs->get_net(sym,line,use);
		assert(!(sym->get_sym_type()==symbol::FUNCTION&&webb!=NULL));
		if(global_reg_map.find(webb)!=global_reg_map.end())
		{
			return global_reg_map.find(webb)->second;
		}
		else
			return NA;
	}
}
void Mem_alloc::allocate_mem()
{
	local_size=0;
	arg_size=0;
	std::vector<symbol*> all_sym=function_tbl->get_all_symbol();
	for(std::vector<symbol*>::iterator it=all_sym.begin();it!=all_sym.end();it++)
	{ 
		symbol::symbol_type typ=(*it)->get_sym_type();
		if(typ==symbol::ARGUMENT||typ==symbol::REFSYM)
		{	
			arg_size+=(*it)->get_size();
			symbol_offset[(*it)]=arg_size;
		}
//		if((*it)->get_sym_type()==symbol::FUNCTION) std::cout<<in_register(*it)<<std::endl;
//		if(typ==symbol::ARRAY) {std::cout<<"inreg "<<in_register(*it)<<std::endl; std::cout<<"enable "<<(*it)->is_enable()<<std::endl;}
		else if((typ==symbol::ARRAY||typ==symbol::FUNCTION||typ==symbol::VARIABLE)&&!in_register(*it)&&(*it)->is_enable())
		{
			local_size+=(*it)->get_size();
			symbol_offset[*it]=local_size;
		}
	}
}
int Mem_alloc::get_offset(symbol* sym)
{
	if(sym->get_sym_type()==symbol::ARGUMENT||sym->get_sym_type()==symbol::REFSYM)
	{
		return -symbol_offset[sym]-local_size-16-(nested_level)*4;
	}
	else return symbol_offset[sym]-local_size;
}
int Mem_alloc::get_disp(int index)	//get display delta with index, begin with 0
{	if(index<0) index=0;
	assert(index<this->nested_level);
	return  -local_size-16-index*4-4;
}
int Mem_alloc::get_nested_level()
{
	return nested_level;
}
int Mem_alloc::get_local_size()
{
	return local_size;
}
int Mem_alloc::get_arg_size()
{
	return arg_size+nested_level*4;
}
bool Mem_alloc::is_alive(symbol* sym, unsigned int line, bool use, unsigned int in_line)
{
	unsigned int index;
	if(sym->get_sym_type()==symbol::FUNCTION) return true;	//these kind of symbol do not have web according to it
	if(sym->get_sym_type()==symbol::CONST) return false;
	if(webs->get_net_index(sym,line, use, index))
	{
		return l_var->get_in(fblk->get_code(in_line)).find(index);
	}
	else
	{		std::cout<<*sym;
			std::cout<<line<<std::endl;
		assert(0);
		return false;
	}
}
//whether a given symbol is in reg
bool Mem_alloc::in_register(symbol* sym)
{
	if(sym->get_sym_type()==symbol::FUNCTION||sym->get_sym_type()==symbol::ARRAY) return false;
	if(global_reg_map.size()==0)
		return false;
	else return symbol_reg_set.find(sym)==symbol_reg_set.end();
}
void Mem_alloc::alloc_global_reg(C_graph* graph)
{
	std::set<symbol*> refered_set;
	std::vector<Quadcode*> inss=fblk->get_all_ins();
	for(int i0=0;i0<inss.size();i0++)
	{
		if(inss[i0]->get_quad_type()==Quadcode::SREF)	//reference symbol shouldn't be assigned with global regs
		{
			refered_set.insert(inss[i0]->get_src_arg()[0]);
		}
	}
	std::map<web*, unsigned int> coloring=graph->get_color(3);	//only 3 global regs
	for(std::map<web*, unsigned int>::iterator it=coloring.begin();it!=coloring.end();it++)
	{
		if(refered_set.find(webs->get_symbol(it->first))!=refered_set.end())
		{
			symbol_reg_set.insert(webs->get_symbol(it->first));
			global_reg_map[it->first]=NA;
			continue;
		}
		switch(it->second)
		{
		case 0:		//do not allocate any global regs
			global_reg_map[it->first]=NA;
			symbol_reg_set.insert(webs->get_symbol(it->first));
			break;
		case 1:
			global_reg_map[it->first]=EBX;
			break;
		case 2:
			global_reg_map[it->first]=EDI;
			break;
		case 3:
			global_reg_map[it->first]=ESI;
			break;
		default:
			assert(0);
		}
	}
}