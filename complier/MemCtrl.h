#ifndef _COMPLIER_MEMCTRL
#define _COMPLIER_MEMCTRL
#pragma warning(disable:4786) 
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <STRING>
#include <SSTREAM>
#include <CASSERT>

#include "SSTable.h"
#include "symbol.h"
#include "SymbolTable.h"
#include "Mem_alloc.h"
#include "FBlock.h"
#include "InsCtrl.h"
#include "Data_stream.h"

/*---------------------------------------
run time stack | temp variables | <---EBP
               | temp variables |
			   |     EDI        |
			   |     ESI        |
			   |     EBP        |
			   |     EBX        |
			   | return addr    |
			   |display(prevEBP)|
			   |     args...    |

*/
const static enum reg_name
{
		NA = 0,
		//GLOBAL
		EBX,
		EBP,
		ESI,
		EDI,
		//LOCAL
		AH,
		AX,
		EAX,
		ECX,
		EDX,
		//SPECIAL
		ESP,
		//
};
const static std::string REG_NAME_MAP[]=
{
	"NA",
	"EBX",
	"EBP",
	"ESI",
	"EDI",
	"AH",
	"AX",
	"EAX",
	"ECX",
	"EDX",
	"ESP",
};
class MemCtrl  
{
public:
	typedef std::pair<unsigned int, bool> par;
	typedef std::pair<symbol*,par> symbol_net;
	MemCtrl(){};
	virtual ~MemCtrl();
	
	MemCtrl(SSTable*, InsCtrl*, Data_stream*);

	std::string get_op_name(symbol*);//
	std::string get_operand(symbol*, unsigned int, bool,SymbolTable*);//
	std::string get_operand(reg_name);//
	std::string get_address(symbol*,SymbolTable*);//
	std::string get_real_address_global(symbol*, SymbolTable*);
	int get_real_address_var(symbol*, SymbolTable*);
	int get_outer_address(symbol*, SymbolTable*);
	std::string get_disp_addr(symbol*, SymbolTable*);
	std::string get_disp_addr(int,SymbolTable*);
	std::set<symbol*> get_const_pool() const;//
	std::set<symbol*> get_global_variable() const;//

	bool is_alive(symbol*, unsigned int, bool, unsigned int);///////
	bool is_outer(symbol* sym, SymbolTable* tbl);
	std::string get_literal_value(symbol*);//
	void increase_offset(int);//
	int getOffset(){return offset;};//

	int get_local_space(symbol*);
	int get_args_space(symbol*);

	int get_nested_lev(symbol*);
	int get_nested_lev(SymbolTable*);

	void occupy_register(reg_name);//
	void occupy_register(reg_name, symbol*, unsigned int, bool);//
	void release_register(reg_name);//
	void prempt_reg(reg_name);
	void release_prempt_reg(reg_name);
	reg_name allocate(symbol*);
	reg_name in_register(symbol*, unsigned int, bool);//
	reg_name in_local_register(symbol*);//
	symbol_net get_symbol_in_reg(reg_name name);
private:
	InsCtrl* ins;
	SSTable* tbl;
	Data_stream* das;

	enum status
	{
		OCCUPIED,
		IDLE,
		PREMPTED,
	};


	std::map<SymbolTable*, Mem_alloc*> stbl_mem_map;
	std::map<symbol*, reg_name> local_reg_map;
	std::map<reg_name, symbol_net> reg_variable_map;
	std::map<reg_name, status> reg_status;
	std::map<symbol*, unsigned int> literal_map;
	std::map<symbol*, std::string> literal_value_map;
	std::set<symbol*> const_pool;
	std::set<symbol*> global_variable;
	std::deque<reg_name> reg_heap;
	int offset;
	std::string get_literal(symbol*);
	unsigned int index;
	void release_reg(reg_name);
	void occupy_reg(reg_name);
	std::set<reg_name> premmepted_regs;
};

#endif