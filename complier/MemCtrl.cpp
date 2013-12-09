#pragma warning(disable:4786)
#include "MemCtrl.h"
#include "utility.h"
#include <SSTREAM>

MemCtrl::MemCtrl(SSTable* table, InsCtrl* insc, Data_stream* _das):das(_das)
{
	ins=insc;
	tbl=table;
	offset=0;
	index=0;
	if(das!=NULL)
	{
		reg_heap.push_back(EAX);	//temporaty regs
		reg_heap.push_back(ECX);
		reg_heap.push_back(EDX);
	}
	else	//data stream = NULL all regs will be regarded as temporary regs
	{
		reg_heap.push_back(EAX);
		reg_heap.push_back(ECX);
		reg_heap.push_back(EDX);
		reg_heap.push_back(EDI);//global
		reg_heap.push_back(ESI);
		reg_heap.push_back(EBX);
	}
	std::vector<FBlock*> blocks = ins->get_all_func_block();	//init the run-time envs
	for(std::vector<FBlock*>::iterator it=blocks.begin();it!=blocks.end();it++)
	{
		SymbolTable* tbl=(*it)->get_function_stable();
		if(das!=NULL)
			stbl_mem_map[tbl]=new Mem_alloc(tbl,(*it),das->get_conflict_graph((*it)),das->get_live_variable((*it)));
		else
			stbl_mem_map[tbl]=new Mem_alloc(tbl,(*it),NULL,NULL);
	}
}

MemCtrl::~MemCtrl()
{

}
//get the name of a given symbol
std::string MemCtrl::get_operand(symbol* sym, unsigned int line, bool use,SymbolTable* cur_tbl)
{
	reg_name name=in_register(sym,line,use);	//the symbol is allocated a global regs, or in temporary regs
	if(name!=NA)
	{
		return get_operand(name);
	}
	else
		return get_address(sym,cur_tbl);	//the address for that operand, only for local ,non array, non reference variables, addressed by ebp
}
std::string MemCtrl::get_address(symbol* sym, SymbolTable* cur_tbl)
{
	std::stringstream ss(std::stringstream::in|std::stringstream::out);
	if(sym->get_sym_type()==symbol::LITERAL)	//literal symbols, was addressed statically
	{
		if(const_pool.find(sym)==const_pool.end())	//add it into const pool if not used. will not be added
		{
			const_pool.insert(sym);
			literal_map[sym]=index++;
			literal_value_map[sym]=utility::to_value_string(sym);
		}
		ss<<"$$"<<get_literal(sym);
	}
	else if(sym->is_global())	//global symbols was addressed statically
	{
		if(sym->get_sym_type()==symbol::CONST)
		{
			if(const_pool.find(sym)==const_pool.end())
			{
				const_pool.insert(sym);
				literal_value_map[sym] = utility::to_value_string(sym);
			}
		}
		if(sym->get_sym_type()==symbol::VARIABLE)
		{
			if(global_variable.find(sym)==global_variable.end())
				global_variable.insert(sym);
		}
		if(sym->get_sym_type()==symbol::ARRAY)
		{
			if(global_variable.find(sym)==global_variable.end())
				global_variable.insert(sym);
		}
		if(sym->get_sym_type()==symbol::FUNCTION)	//a function is allocated memory for return values
		{
			if(global_variable.find(sym)==global_variable.end())
				global_variable.insert(sym);
			ss<<"$"<<sym->get_id()<<"@"<<sym->get_index();
			return ss.str();
		}
		ss << "dword ptr $" << sym->get_id() << "@global"<<sym->get_scope()->get_scope()->get_index();
	}
	else if(sym->get_sym_type()==symbol::CONST)
	{
		if(const_pool.find(sym)==const_pool.end())
		{
			const_pool.insert(sym);
			literal_value_map[sym]=utility::to_value_string(sym);
		}
		ss << "$" << sym->get_id()<< sym->get_scope()->get_name()<<sym->get_scope()->get_scope()->get_index();
	}
	else	//local, non array, non reference variables, can be addressed by ebp statically
	{
		std::string scope1;
		std::string scope2;
		scope1=utility::get_func_name(sym->get_scope()->get_scope());
		scope2=utility::get_func_name(cur_tbl->get_scope());
		int delta=0;
		//while(scope1!=scope2)//the symbol is not in the current scope
		//{
		//	Mem_alloc* _mem=stbl_mem_map[cur_tbl];

//			delta+=(20+_mem->get_arg_size()+_mem->get_local_size());
//			scope2=utility::get_func_name(cur_tbl->get_before_table()->get_scope());
//		}
		assert(scope1==scope2); //only for variable in same cur scope
		int address=stbl_mem_map[sym->get_scope()]->get_offset(sym)- delta;// +offset;
		if(address>0)
			ss<<"dword ptr [ebp] - "<<address;
		else if(address<0)
			ss<<"dword ptr [ebp] + "<<-address;
		else
			ss<<"dword ptr [ebp]";
	}
	return ss.str();
}
//get real address of a given symbol, used for reference
std::string MemCtrl::get_real_address_global(symbol* sym, SymbolTable* cur_tbl)
{
	assert(sym->get_sym_type()!=symbol::LITERAL);
	assert(sym->get_sym_type()!=symbol::CONST);
	assert(sym->is_global());
	std::stringstream ss(std::stringstream::in|std::stringstream::out);
	if(sym->get_sym_type()==symbol::VARIABLE)
	{
		if(global_variable.find(sym)==global_variable.end())
			global_variable.insert(sym);
	}
	if(sym->get_sym_type()==symbol::ARRAY)
	{	//std::cout<<sym<<std::endl;
		if(global_variable.find(sym)==global_variable.end())
			global_variable.insert(sym);
	}
	ss << "offset " <<"$"<< sym->get_id() << "@global"<<sym->get_scope()->get_scope()->get_index();
	return ss.str();
}
//get address delta for a given sybol, only for local temp variables, used for reference
int MemCtrl::get_real_address_var(symbol* sym, SymbolTable* cur_tbl)
{
	assert(!sym->is_global());
	
	std::string scope1;
	std::string scope2;
	scope1=utility::get_func_name(sym->get_scope()->get_scope());
	scope2=utility::get_func_name(cur_tbl->get_scope());
	int delta=0;
	assert(scope1==scope2);
// 	while(scope1!=scope2)//the symbol is not in the current scope
// 	{
// 		Mem_alloc* _mem=stbl_mem_map[cur_tbl];
// 			
// 		delta+=(20+_mem->get_arg_size()+_mem->get_local_size());
// 		scope2=utility::get_func_name(cur_tbl->get_before_table()->get_scope());
// 	}
	int address=stbl_mem_map[sym->get_scope()]->get_offset(sym)- delta;

	return address;
}
int MemCtrl::get_outer_address(symbol* sym, SymbolTable* cur_tbl) //get the address delta for outer symbols, the added with display address
{
	return stbl_mem_map[sym->get_scope()]->get_offset(sym);
}
std::string MemCtrl::get_disp_addr(symbol* sym, SymbolTable* cur_tbl)	//get the base addr can be addressed statically
{
	std::stringstream ss(std::stringstream::in|std::stringstream::out); //get display area address, the display area contains outer ebp
	int lev1=stbl_mem_map[sym->get_scope()]->get_nested_level(); //1
	int lev2=stbl_mem_map[cur_tbl]->get_nested_level();//2
	assert(lev2>lev1);
	int addr=stbl_mem_map[cur_tbl]->get_disp(lev2-lev1-1);	//can be accessed by ebp
	if(addr<0)
		ss<<"dword ptr [ebp] + "<<-addr;
	else if(addr>0)
		ss<<"dword ptr [ebp] - "<<addr;
	else
		ss<<"dword ptr [ebp]";
	return ss.str();
}
std::string MemCtrl::get_disp_addr(int lev_delta, SymbolTable* cur_tbl) //begin with 0, outer most
{
	std::stringstream ss(std::stringstream::in|std::stringstream::out);
	assert(lev_delta<=stbl_mem_map[cur_tbl]->get_nested_level());
	int addr=stbl_mem_map[cur_tbl]->get_disp(lev_delta);
	if(addr<0)
		ss<<"dword ptr [ebp] + "<<-addr;
	else if(addr>0)
		ss<<"dword ptr [ebp] - "<<addr;
	else
		ss<<"dword ptr [ebp]";
	return ss.str();
}
bool MemCtrl::is_outer(symbol* sym, SymbolTable* cur_tbl)	//whether a given symbol is in outer level
{
	int lev1,lev2;
	symbol::symbol_type typ;
	typ=sym->get_sym_type();
	if(typ!=symbol::LITERAL&&typ!=symbol::CONST&&!sym->is_global())	//the global and literal is not considered as outer
		lev1=stbl_mem_map[sym->get_scope()]->get_nested_level();
	else return false;
	lev2=stbl_mem_map[cur_tbl]->get_nested_level();
	return lev1<lev2;
}
std::string MemCtrl::get_literal_value(symbol* sym)
{
	return literal_value_map[sym];
}
std::string MemCtrl::get_operand(reg_name name)
{
	switch(name)
	{
	case EAX:
		return "EAX";
	case EBP:
		return "EBP";
	case EBX:
		return "EBX";
	case ESI:
		return "ESI";
	case EDI:
		return "EDI";
	case AH:
		return "AH";
	case AX:
		return "AX";
	case ECX:
		return "ECX";
	case EDX:
		return "EDX";
			//SPECIAL
	case ESP:
		return "ESP";
	default:
		assert(0);
	}
	return "";
}
std::set<symbol*> MemCtrl::get_const_pool() const
{
	return const_pool;
}
std::set<symbol*> MemCtrl::get_global_variable() const
{
	return global_variable;
}
void MemCtrl::increase_offset(int val)
{
	offset -=val;
}
reg_name MemCtrl::in_register(symbol* sym, unsigned int line, bool use)		//whether a given symbol is allocated a regs
{
	reg_name reg=in_local_register(sym);	//in local temporary regs
	if(reg!=NA)
		return reg;
	else
	{
		if(sym->get_sym_type()==symbol::LITERAL||sym->is_global())	//global literal
		{
			return NA;
		}
		else
		{
			return stbl_mem_map[sym->get_scope()]->in_register(sym,line,use);	//may allocated global regs
		}
	}
}
reg_name MemCtrl::in_local_register(symbol* sym)	//whether a given symbol is allocated temporary regs
{
	std::map<symbol*, reg_name>::iterator it=local_reg_map.find(sym);	//<symbol*, reg_name>
	if(it!=local_reg_map.end())
		return it->second;
	else
		return NA;
}
std::string MemCtrl::get_literal(symbol* sym)
{
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	switch(sym->get_val_type())
	{
	case symbol::CHAR:
		ss<<"char$";
		break;
	case symbol::INTEGER:
		ss<<"int$";
		break;
	case symbol::STRING:
		ss<<"string$";
		break;
	}
	
	ss<<literal_map[sym];//contain the index of all the literal
	return ss.str();
}

std::string MemCtrl::get_op_name(symbol* sym)		//get the name of a given symbol
{	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	switch (sym->get_sym_type())
	{
	case symbol::LITERAL:
				  ss<<"$$"<<get_literal(sym);
				  break;
	case symbol::LABEL:
				 ss<<"@@"<<sym->get_id();
				 break;
	case symbol::FUNCTION:
	case symbol::PROCEDURE:
				 ss<<"$$"<<sym->get_id()<<"@"<<sym->get_index();	//use index to differentiate the function|procedure with same name
				 break;
	case symbol::VARIABLE:
				assert(sym->is_global());
				ss<<"$"<<sym->get_id()<<sym->get_scope()->get_name()<<sym->get_scope()->get_scope()->get_index();
				break;
	case symbol::CONST:
				ss<<"$"<<sym->get_id()<<sym->get_scope()->get_name()<<sym->get_scope()->get_scope()->get_index();
				break;
	case symbol::ARRAY:
				ss<<"$"<<sym->get_id()<<sym->get_scope()->get_name()<<sym->get_scope()->get_scope()->get_index();
				break;
		default:
			;
	}
	assert(ss.str().size());
	return ss.str();
}
//whether a given symbol is alive, if not, not necessary to write back the regs
bool MemCtrl::is_alive(symbol* sym, unsigned int line, bool use, unsigned int in_line)
{
	assert(sym->get_scope()->get_name()!="@global");
	return stbl_mem_map[sym->get_scope()]->is_alive(sym,line,use,in_line);
}
void MemCtrl::occupy_register(reg_name name)	//allocate a local regs
{
	if(reg_variable_map.find(name) != reg_variable_map.end()) //modify the local data structures for regs and symbols
	{
		symbol_net prev = reg_variable_map[name];
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;
	}
	reg_variable_map[name].first = NULL;
	reg_status[name] = OCCUPIED;
	occupy_reg(name);
}
//allocate a local reg for a given symbol
void MemCtrl::occupy_register(reg_name reg, symbol* sym, unsigned int line, bool is_use)
{ 
	if(reg_variable_map.find(reg) != reg_variable_map.end())//reg_variable_map <reg, symbol*>
	{
		symbol_net prev = reg_variable_map[reg];//find a symbol in reg
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;  //local reg map <symbol*, reg>
	}	
	if(local_reg_map[sym] == NA)//the symbol do not a reg
	{  
		local_reg_map[sym] = reg; //put it into <symbol, reg>
		reg_variable_map[reg] = symbol_net(sym, std::pair<unsigned int, bool>(line, is_use));//reg variable
		reg_status[reg] = OCCUPIED;
		occupy_reg(reg);
	}
	else //symbol have already got a reg, get it another reg
	{  
		release_reg(local_reg_map[sym]);
		occupy_register(reg,sym,line,is_use);
	}
}
int MemCtrl::get_nested_lev(symbol* sym)	//get the nested level for a procedure|function, can be used to decide the size of the display
{
	if(sym->get_sym_type()==symbol::PROCEDURE||sym->get_sym_type()==symbol::FUNCTION)
		return stbl_mem_map[sym->get_cur_scope()]->get_nested_level();
	else
		return stbl_mem_map[sym->get_scope()]->get_nested_level();
}
int MemCtrl::get_nested_lev(SymbolTable* cur_tbl)
{
	return stbl_mem_map[cur_tbl]->get_nested_level();
}
void MemCtrl::release_register(reg_name name)	//release a local regs
{
	release_reg(name);
	if(reg_variable_map.find(name) != reg_variable_map.end())	//modify the local data structure
	{  
		symbol_net prev = reg_variable_map[name];
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;
	}
	reg_variable_map[name].first = NULL;
	reg_status[name ] = IDLE;
	
}
int MemCtrl::get_local_space(symbol* sym)	//get the local variable size, used for allocate memory
{
	assert(sym->get_sym_type()==symbol::FUNCTION||sym->get_sym_type()==symbol::PROCEDURE);
	if(sym->get_index()==0) return 0; //index is the index of the function
	SymbolTable* scope = sym->get_cur_scope();
	return stbl_mem_map[scope]->get_local_size();
}
int MemCtrl::get_args_space(symbol* sym)	//get the size of args, can be used for ret clause
{
	assert(sym->get_sym_type()==symbol::PROCEDURE||sym->get_sym_type()==symbol::FUNCTION);
	if(sym->get_index()==0) return 0;
	SymbolTable* scope=sym->get_cur_scope();
	return stbl_mem_map[scope]->get_arg_size();
}
typedef std::pair<unsigned int, bool> par;
typedef std::pair<symbol*,par> symbol_net;
symbol_net MemCtrl::get_symbol_in_reg(reg_name reg)		//whether a given reg was allocated to a symbol
{
	return reg_variable_map[reg];
}
void MemCtrl::prempt_reg(reg_name name)		//preempt a local regs, before it was released, should not be assigned to other symbols 
{//	std::cout<<REG_NAME_MAP[name]<<" prempted"<<std::endl;
	occupy_register(name);
	premmepted_regs.insert(name);
}
void MemCtrl::release_prempt_reg(reg_name name)	//release a preempt local regs
{  //	std::cout<<REG_NAME_MAP[name]<<"   released"<<std::endl;
	release_register(name);
	premmepted_regs.erase(premmepted_regs.find(name));
}
reg_name MemCtrl::allocate(symbol* sym)	//allocate a local reg for a symbol
{
	reg_name reg = in_local_register(sym);
	if(reg == NA)
	{	assert(premmepted_regs.find(reg_heap.front())==premmepted_regs.end());	//should not allocate preempted regs
		reg = reg_heap.front();
	}
	return reg;
}

void MemCtrl::release_reg(reg_name name)	//release a reg, put it in the front
{
	for(std::deque<reg_name>::iterator it = reg_heap.begin(); it != reg_heap.end(); it++)
	{
		if(*it==name)
		{
			reg_heap.erase(it);
			reg_heap.push_front(name);
			break;
		}
	}

}
void MemCtrl::occupy_reg(reg_name name)	//occupy a local reg, put it in the back of the heap
{
	for(std::deque<reg_name>::iterator it=reg_heap.begin();it!=reg_heap.end();it++)
	{
		if(*it==name)
		{
			reg_heap.erase(it);
			reg_heap.push_back(name);
			break;
		}
	}
}