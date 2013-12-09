#ifndef _COMPILER_BBLOCK
#define _COMPILER_BBLOCK
#pragma warning(disable:4786)
#include "Quadcode.h"
#include "SymbolTable.h"
#include <VECTOR>
#include <IOSTREAM>
#include <MAP>

class BBlock  
{
public:

friend class Dag;
friend class Reach_def;
friend class Live_Var;

	BBlock(SymbolTable*);
void add_code(Quadcode* q);

void set_entry(BBlock* block);
void set_exit(BBlock* block);
static void reset();
bool has_array();
std::vector<Quadcode*> get_all_ins() const;

Quadcode* operator[](unsigned int);
friend void operator<<(std::ostream&, const BBlock&);
private:
	std::vector<Quadcode*> all_ins;
	std::vector<BBlock*> entry;
	std::vector<BBlock*> exit;
	std::map<unsigned int, unsigned int> index_map;
	unsigned int number;
	static unsigned int instance_number;
	SymbolTable* scope;
};

#endif