#ifndef _COMPLIER_MEM_ALLOC
#define _COMPLIER_MEM_ALLOC
#pragma warning(disable:4786)
#include <VECTOR>
#include <MAP>
#include "SSTable.h"
#include "SymbolTable.h"
#include "symbol.h"
#include "Data_stream.h"

// class MemCtrl;
 enum reg_name;

class Mem_alloc  
{
public:
	Mem_alloc(SymbolTable*, FBlock*, C_graph*, Live_Var*);
	void allocate_mem();
	int get_offset(symbol*);
	reg_name in_register(symbol*, unsigned int, bool) const;
	int get_local_size();
	int get_arg_size();
	bool is_alive(symbol*, unsigned int, bool, unsigned int);
	int get_disp(int);
	int get_nested_level();
private:
	void alloc_global_reg(C_graph* graph);
	bool in_register(symbol*);
	FBlock* fblk;
	Live_Var* l_var;
	Webs* webs;
	
	std::map<web*, reg_name> global_reg_map;
	std::set<symbol*> symbol_reg_set;

	std::map<symbol*,int> symbol_offset;
	int arg_size;
	int local_size;
	int nested_level;
	SymbolTable* function_tbl;
};

#endif
