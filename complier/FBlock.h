#ifndef _COMPILER_FBLOCK
#define _COMPILER_FBLOCK
#pragma warning(disable:4786)
#include "BBlock.h"
#include "Quadcode.h"
#include "SymbolTable.h"
#include <VECTOR>
#include <IOSTREAM>
#include <MAP>

class FBlock  
{
public:

	friend class Dag;

	FBlock(SymbolTable*);


	friend void operator<<(std::ostream&, const FBlock&);
	void add_ins(Quadcode*);
	static	void reset();

	std::vector<BBlock*> get_basic_blocks() const;
	std::vector<Quadcode*> get_all_ins() const;
	SymbolTable* get_function_stable();
	Quadcode* get_code(unsigned int);

private:
	std::vector<BBlock*> all_blocks;
	std::map<symbol*, BBlock*> block_entry;//map the function entry to basic block entry
	std::map<BBlock*, symbol*> block_exit;//map the block to an exit
	std::vector<BBlock*> return_blocks;
	std::vector<Quadcode*> all_ins;
	void flow_BBlock();
	void connect_BBlock(BBlock* entry,BBlock* exit);
	std::map<unsigned int, unsigned int> index_map;


	bool new_block;//a new block formed
	bool following_label;//is have following label
	bool is_end;//is end?
	BBlock* former_block;//the former block
	SymbolTable* cur_func_tbl;

	unsigned int number;
	static unsigned int instance_number;
	
	void flow(BBlock*, BBlock*);
	void flow();
	void new_basic_block();
	void map_label_to_block(symbol*, BBlock*);
};
#endif