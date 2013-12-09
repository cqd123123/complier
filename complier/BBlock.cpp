#pragma warning(disable:4786)
#include "BBlock.h"
#include "utility.h"

unsigned int BBlock::instance_number = 0;

BBlock::BBlock(SymbolTable* _scope)
{
	number = instance_number++;
	scope=_scope;
}
void BBlock::add_code(Quadcode* q)
{
	index_map[q->get_index()] = all_ins.size();
	all_ins.push_back(q);
}
std::vector<Quadcode*> BBlock::get_all_ins() const
{
	return all_ins;
}
void BBlock::set_entry(BBlock* block)
{
	entry.push_back(block);
}

void BBlock::set_exit(BBlock* block)
{
	exit.push_back(block);
}

void BBlock::reset()
{
	instance_number=0;
}
bool BBlock::has_array()
{
	for(int i=0;i<all_ins.size();i++)
	{
		std::vector<symbol*> sym_array=all_ins[i]->get_all_sym();
		for(int j=0;j<sym_array.size();j++)
		{
			if(sym_array[j]->get_sym_type()==symbol::ARRAY||sym_array[j]->get_sym_type()==symbol::REFSYM||sym_array[j]->get_sym_type()==symbol::FUNCTION||
				(utility::is_arthimethic(all_ins[i])&&sym_array[j]->get_scope()!=scope))
				return true;
		}
	}
	return false;
}
void operator<<(std::ostream& out, const BBlock& block)
{
	out << "Basic Block " << block.number << std::endl;
	out << "Entry:";
	for(unsigned int i = 0; i < block.entry.size(); i++)
		out << " " << block.entry[i]->number;
	out << std::endl;
	out << "Exit:";
	for(unsigned int j = 0; j < block.exit.size(); j++)
		out << " " << block.exit[j]->number;
	out << std::endl;
	for(unsigned int k = 0; k < block.all_ins.size(); k++)
		out << *block.all_ins[k];
	out << std::endl;
}
Quadcode* BBlock::operator[](unsigned int index)
{
	return all_ins[index_map[index]];
}

