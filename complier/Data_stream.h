#ifndef _COMPILER_DATA_STREAM
#define _COMPILER_DATA_STREAM
#pragma warning(disable:4786)
#include <VECTOR>
#include <MAP>
#include <SET>
#include "InsCtrl.h"
#include "Quadcode.h"
#include "BBlock.h"
#include "FBlock.h"
#include "symbol.h"
#include "SymbolTable.h"

class bit_set	//bit set, bit vector
{
public:
	bit_set operator+(const unsigned int&);
	bit_set operator+(const bit_set&);//bit_set union
	bit_set& operator+=(const unsigned int&);
	bit_set& operator+=(const bit_set&);
	bit_set operator-(const unsigned int&);
	bit_set operator-(const bit_set&);//bit_set sub
	bit_set& operator-=(const unsigned int&);
	bit_set& operator-=(const bit_set&);
	bit_set operator&(const bit_set&);//bit_set intersect
	bit_set& operator&=(const bit_set&);
	
	bool empty() const;
	bool find(unsigned int) const;
	std::vector<unsigned int> get_numbers() const;
	friend bool operator==(const bit_set&, const bit_set&);
	friend bool operator!=(const bit_set&, const bit_set&);
	friend void operator<<(std::ostream&, const bit_set&);
private:
	std::vector<unsigned int> members;
	const static unsigned int bits_per_byte;
	const static unsigned int bits_per_int;
};
class Reach_def		//reach definition
{
public:
	Reach_def(FBlock*, std::map<symbol*, bit_set>, std::map<symbol*, bit_set>);
	bit_set get_in_r_def(Quadcode*) const;
	bit_set get_out_r_def(Quadcode*) const;
private:
	FBlock* fblk;
	std::map<symbol*, bit_set>& def;
	std::map<symbol*, bit_set>& use;
	std::map<Quadcode*, bit_set> code_in;
	std::map<Quadcode*, bit_set> code_out;
	void _analyse();
};

/* after the nets are constructed by reach definition, all the webs are considered as individual units for memory
allocation, instead of symbols. a symbol can have multiple webs, a web have only one symbol according to it.
A use point in the program is only contained in one web, same symbol in same line of code can contain in different
webs.*/
typedef std::map<unsigned int, bit_set> chains;
typedef std::pair<unsigned int, bit_set> chain;		//indexed by define line
typedef std::vector<chain> web;
typedef std::pair<symbol*, web*> sym_web;
typedef std::pair<symbol*, unsigned int> sym_line;

class Webs
{
public:
	Webs(FBlock*, std::map<symbol*, bit_set>, std::map<symbol*, bit_set>, Reach_def*);

	std::vector<sym_web> get_web_vec() const;
	symbol* get_symbol(web*);
	web* get_net(symbol*, unsigned int, bool) const;
	unsigned int get_net_index(web*) const;
	bool get_net_index(symbol*, unsigned int, bool, unsigned int&) const;

private:
	void _analyse();
	void merge(chains&);
	void mapping();

	void mapping(symbol*, unsigned int, bool);
	bool is_consisent_define(symbol*, unsigned int, web* _web);
	bool is_consisent_use(symbol*, unsigned int, web* web);
	FBlock* fblk;
	Reach_def* r_def;
	std::map<symbol*, bit_set>& use;
	std::map<symbol*, bit_set>& def;
	std::multimap<symbol*, web*> symbol_web_map;	// a symbol can have multiple webs
	std::vector<sym_web> web_vec;
	std::map<web*, unsigned int> web_index_map;
	std::map<sym_line, unsigned int> sym_uweb_map;	//a use symbol and line code
	std::map<sym_line, unsigned int> sym_dweb_map;
};


class Live_Var
{
public: Live_Var(FBlock*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&, Webs*);
		bit_set get_in(Quadcode*) const;
		bit_set get_out(Quadcode*) const;
private:
		void _analyse();
		void elimiate();
		void define_or_use(symbol*, Quadcode*, std::set<unsigned int>&, bit_set&, std::map<Quadcode*, bit_set>&, bool);
		Webs* webs;
		FBlock* fblk;
		std::map<symbol*, bit_set>& use;
		std::map<symbol*, bit_set>& define;
		std::map<Quadcode*, bit_set> code_in;
		std::map<Quadcode*, bit_set> code_out;
};

typedef std::vector<bool> bool_vec;

class C_graph{
public:
	C_graph(FBlock*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&, Reach_def*, Webs*, Live_Var*);
	std::map<web*, unsigned int> get_color(unsigned int reg_num);
	Webs* get_Webs() const;

private:
	void _bulid();
	void _get_reference();
	void pre_scan();	//pre_scan, get the refered symbol;
	std::set<symbol*> refered_symbol;	//those symbol which was refered to should not be assigned with global regs
	std::map<web*, unsigned int> color_map;
	std::map<web*, unsigned int> refs;

	FBlock* fblk;
	SymbolTable* cur_stbl;	//current symbol table, used for find outer symbols
	std::map<symbol*, bit_set>& use;	//the symbol definition lines
	std::map<symbol*, bit_set>& define;
	Reach_def* r_def;
	Webs* webs;
	Live_Var* l_var;

	bool flag;
	std::vector<bool_vec> adjecent_graph;
	void coloring(unsigned int);
	unsigned int find_degree_less_than(unsigned int, bool_vec&);
	unsigned int choose_no_reg(std::vector<sym_web>&, std::vector<bool>&);
	unsigned int allocate_color(unsigned int, unsigned int, std::vector<sym_web>&, std::vector<bool>&);
	bool is_conflict(Quadcode*, unsigned int, chain);
};


class Data_stream	//global data_stream  
{
public:
	Data_stream(InsCtrl*);
	Live_Var* get_live_variable(FBlock*) const;
	C_graph* get_conflict_graph(FBlock*) const;
private:
	InsCtrl* ins;
	std::map<FBlock*, Live_Var*> l_var; 
	std::map<FBlock*, C_graph*> c_graph;
	std::map<symbol*, bit_set> definition;	//symbol definition points
	std::map<symbol*, bit_set> use;	//symbol use points
	void pre_scan(FBlock*);
	void _analyse();
};

#endif